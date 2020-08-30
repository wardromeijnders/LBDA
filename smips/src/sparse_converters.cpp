#include <armadillo>
#include <carma/carma.h>
#include <pybind11/pybind11.h>

// TODO make this work


/**
 * SCIPY TO ARMADILLO
 */
template<typename T>
arma::SpMat<T> csc_to_sp_mat(py::handle src, bool copy = false, bool strict = false)
{
    /* Convert scipy csc_matrix to Armadillo sparse matrix
     *
     * The default behaviour is to avoid copying, we copy if:
     * - ndim == 2 && not F contiguous memory
     * - writeable is false
     * - owndata is false
     * - memory is not aligned
     * Note that the user set behaviour is overridden is one of the above
     * conditions is true
     *
     * If the array is 1D we create a column oriented matrix (N, 1)
     */

    // set as array buffer
    py::array_t<T> buffer = py::array_t<T>::ensure(src);

    if (!buffer)
    {
        throw std::runtime_error("armadillo matrix conversion failed");
    }

    auto dims = buffer.ndim();

    if (dims > 2)
    {
        throw std::runtime_error("Number of dimensions must be 2");
    }

    py::buffer_info info = buffer.request();

    if (info.ptr == nullptr)
    {
        throw std::runtime_error("armadillo matrix conversion failed, nullptr");
    }

#ifdef CARMA_DONT_REQUIRE_F_CONTIGUOUS
    if (requires_copy(buffer))
    {
        copy = false;
        strict = false;
    }
#else
    if (requires_copy(buffer) || !is_f_contiguous(buffer))
    {
        // If not F-contiguous or writeable or numpy's data let pybind handle the copy
        buffer = py::array_t<T, py::array::f_style | py::array::forcecast>::ensure(src);
        info = buffer.request();
        copy = false;
        strict = false;
    }
#endif

    return arma::Mat<T>(static_cast<T *>(info.ptr),
                        info.shape[0],
                        info.shape[1],
                        copy,
                        strict);
}

/**
 * ARMADILLO TO SCIPY
 */
template <typename T>
inline py::array_t<T> sp_mat_to_csc(arma::SpMat<T>* src, bool copy) {
    /* Convert armadillo matrix to scipy.sparse csc array */
    ssize_t tsize = static_cast<ssize_t>(sizeof(T));
    ssize_t nrows = static_cast<ssize_t>(src->n_rows);
    ssize_t ncols = static_cast<ssize_t>(src->n_cols);

    auto data = get_data<arma::SpMat<T>>(src, copy);
    py::capsule base = create_capsule(data);

    return py::array_t<T>(
        {nrows, ncols},          // shape
        {tsize, nrows * tsize},  // F-style contiguous strides
        data.data,               // the data pointer
        base                     // numpy array references this parent
    );
}
