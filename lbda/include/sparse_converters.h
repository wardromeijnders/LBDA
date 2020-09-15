#ifndef SMIPS_SPARSE_CONVERTERS_H
#define SMIPS_SPARSE_CONVERTERS_H

#include <armadillo>
#include <carma/carma.h>
#include <pybind11/pybind11.h>

/**
 * SCIPY TO ARMADILLO
 */
template<typename T>
arma::SpMat<T> csc_to_sp_mat(py::handle &src,
                             bool copy = false,
                             bool strict = false)
{
    // TODO incref/decref
    PyObject *colPtrRaw = src.attr("indptr").ptr();
    arma::uvec colPtr = carma::arr_to_col<arma::uword>(colPtrRaw, copy, strict);

    PyObject *rowindRaw = src.attr("indices").ptr();
    arma::uvec rowind = carma::arr_to_col<arma::uword>(rowindRaw, copy, strict);

    PyObject *dataRaw = src.attr("data").ptr();
    arma::Col<T> data = carma::arr_to_col<T>(dataRaw, copy, strict);

    PyObject *shapePtr = src.attr("shape").ptr();

    int row, col;
    PyArg_ParseTuple(shapePtr, "ii", &row, &col);

    return arma::SpMat<T>(rowind, colPtr, data, row, col);
}

/**
 * ARMADILLO TO SCIPY
 */
template<typename T> inline py::handle sp_mat_to_csc(arma::SpMat<T> const &src)
{
    // TODO incref/decref
    src.sync();  // ensures the following data accesses are valid.

    py::array_t<T> data(src.n_nonzero, src.values);
    py::array_t<arma::uword> indices(src.n_nonzero, src.row_indices);
    py::array_t<arma::uword> indPtr(src.n_cols + 1, src.col_ptrs);

    if (!data || !indices || !indPtr)
        throw std::runtime_error("Could not convert matrix data.");

    PyObject *args = Py_BuildValue("((OOO))",
                                   data.ptr(),
                                   indices.ptr(),
                                   indPtr.ptr());

    if (!args)
        throw std::runtime_error("Could not construct matrix arguments.");

    PyObject *shape = Py_BuildValue("ii", src.n_rows, src.n_cols);
    PyObject *kwargs = PyDict_New();

    if (!kwargs || PyDict_SetItemString(kwargs, "shape", shape) == -1)
        throw std::runtime_error("Could not set shape kwarg.");

    py::module sparse = py::module::import("scipy.sparse");
    py::object csc_matrix = sparse.attr("csc_matrix");

    PyObject *mat = PyObject_Call(csc_matrix.ptr(), args, kwargs);

    if (!mat)
        throw std::runtime_error("Could not construct scipy.sparse matrix.");

    return mat;
}

namespace pybind11::detail
{
    template<typename T> struct type_caster<arma::SpMat<T>>
    {
    public:
        PYBIND11_TYPE_CASTER(arma::SpMat<T>,
                             _("scipy.sparse.csc.csc_matrix[")
                                 + npy_format_descriptor<T>::name + _("]"));

        bool load(handle src, bool)
        {
            value = csc_to_sp_mat<T>(src);
            return !PyErr_Occurred();
        }

        static handle cast(arma::SpMat<T> const &src,
                           return_value_policy,
                           handle)
        {
            return sp_mat_to_csc(src);
        }
    };
}  // namespace pybind11::detail

#endif  // SMIPS_SPARSE_CONVERTERS_H
