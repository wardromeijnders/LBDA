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
    PyObject *raw = src.ptr();

    PyObject *colPtrRaw = PyObject_GetAttrString(raw, "indptr");
    arma::uvec colPtr = carma::arr_to_col<arma::uword>(colPtrRaw, copy, strict);

    PyObject *rowindRaw = PyObject_GetAttrString(raw, "indices");
    arma::uvec rowind = carma::arr_to_col<arma::uword>(rowindRaw, copy, strict);

    PyObject *dataRaw = PyObject_GetAttrString(raw, "data");
    arma::vec data = carma::arr_to_col<double>(dataRaw, copy, strict);

    PyObject *shapePtr = PyObject_GetAttrString(raw, "shape");

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
    py::module sparse = py::module::import("scipy.sparse");
    py::object csc_matrix = sparse.attr("csc_matrix");

    // TODO make the conversions work nicely
    py::array_t<T> data = carma::to_numpy<T>(src.values, true);
    py::array_t<T> indices = carma::to_numpy<T>(src.row_indices, true);
    py::array_t<T> indPtr = carma::to_numpy<T>(src.col_ptrs, true);

    PyObject *args
        = Py_BuildValue("(ooo)(ii)s",
                        data,
                        indices,
                        indPtr,
                        src.n_rows,
                        src.n_cols,
                        pybind11::detail::npy_format_descriptor<T>::name);

    PyObject *mat = PyObject_CallObject(csc_matrix.ptr(), args);

    return py::handle(mat);
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

        static handle cast(arma::SpMat<T> &src, return_value_policy, handle)
        {
            return sp_mat_to_csc(src);
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
