#ifndef SMIPS_SPARSE_CONVERTERS_H
#define SMIPS_SPARSE_CONVERTERS_H

#include <armadillo>
#include <carma/carma.h>
#include <pybind11/pybind11.h>

// TODO make this work completely


/**
 * SCIPY TO ARMADILLO
 */
template<typename T>
arma::SpMat<T> csc_to_sp_mat(py::handle &src,
                             bool copy = false,
                             bool strict = false)
{
    // TODO incref/decref
    auto raw = src.ptr();

    auto indPtrRaw = PyObject_GetAttrString(raw, "indptr");
    py::handle indHandle(indPtrRaw);
    arma::uvec indPtr = carma::arr_to_col<arma::uword>(indHandle);

    auto indicesRaw = PyObject_GetAttrString(raw, "indices");
    py::handle indicesHandle(indicesRaw);
    arma::uvec indices = carma::arr_to_col<arma::uword>(indicesHandle);

    auto dataRaw = PyObject_GetAttrString(raw, "data");
    py::handle dataHandle(dataRaw);
    arma::vec data = carma::arr_to_col<double>(dataHandle);

    auto shapePtr = PyObject_GetAttrString(raw, "shape");

    auto row = PyTuple_GetItem(shapePtr, 0);
    auto col = PyTuple_GetItem(shapePtr, 1);

    return arma::SpMat<T>(indices,
                          indPtr,
                          data,
                          PyLong_AsLong(row),
                          PyLong_AsLong(col));
}

/**
 * ARMADILLO TO SCIPY
 */
 template<typename T>
 inline py::array_t<T> sp_mat_to_csc(arma::SpMat<T> *src, bool copy)
{
    /* Convert armadillo matrix to scipy.sparse csc array */
    ssize_t tsize = static_cast<ssize_t>(sizeof(T));
    ssize_t nrows = static_cast<ssize_t>(src->n_rows);
    ssize_t ncols = static_cast<ssize_t>(src->n_cols);

    auto data = get_data<arma::SpMat<T>>(src, copy);
    py::capsule base = create_capsule(data);

    return py::array_t<T>({nrows, ncols},          // shape
                          {tsize, nrows * tsize},  // F-style contiguous
                          strides data.data,               // the data pointer
                          base  // numpy array references this parent
    );
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
            // TODO
            return handle();
        }

        static handle cast(arma::SpMat<T> const &src,
                           return_value_policy,
                           handle)
        {
            // TODO
            return handle();
        }
    };
}  // namespace pybind11::detail

#endif  // SMIPS_SPARSE_CONVERTERS_H
