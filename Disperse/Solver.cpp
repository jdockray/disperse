
#include "Exceptions.hpp"
#include "Solver.hpp"
#include "SolverSession.hpp"

class CompressedSparseColumnMatrix {
public:
    CompressedSparseColumnMatrix(const SparseMatrix& matrix) {
        SparseMatrix transposed = getTranspose(matrix);
        // std::map is ordered so it is already sorted by the key
        for (const auto column : transposed.matrixElements()) {
            for (const auto cell : column.second) {
                while (columnPointers.size() <= column.first) {
                    columnPointers.push_back(values.size());
                }
                rowIndices.push_back(cell.first);
                values.push_back(cell.second);
            }
        }
        while (columnPointers.size() <= matrix.columnCount()) {
            columnPointers.push_back(rowIndices.size());
        }
        std::memset(&cscStruct, 0, sizeof(cscStruct));
        cscStruct.nzmax = rowIndices.size();
        cscStruct.m = matrix.rowCount();
        cscStruct.n = matrix.columnCount();
        cscStruct.p = columnPointers.data();
        cscStruct.i = rowIndices.data();
        cscStruct.x = values.data();
        cscStruct.nz = -1;
    }

    csc* getCSCPtr() {
        return &cscStruct;
    }

private:
    CompressedSparseColumnMatrix(CompressedSparseColumnMatrix const&) = delete;
    CompressedSparseColumnMatrix& operator=(CompressedSparseColumnMatrix const&) = delete;

    std::vector<c_int> columnPointers;
    std::vector<c_int> rowIndices;
    std::vector<c_float> values;
    csc cscStruct;
};

OSQPSession::~OSQPSession() {
    if (pOsqpWorkspace) {
        osqp_cleanup(pOsqpWorkspace);
    }
};

OSQPSolver::OSQPSolver() {
    osqp_set_default_settings(&osqp_settings);
    osqp_settings.max_iter = static_cast<c_int>(rounding_multipler * 100); // maximum iterations to take
    osqp_settings.eps_abs = static_cast<c_float>(0.1 / rounding_multipler); // Absolute convergence tolerance
    osqp_settings.eps_rel = osqp_settings.eps_abs; // Relative convergence tolerance
}

// Optimisation minimises x'Px subject to l<=Ax<=u
std::vector<double> OSQPSolver::solve(const SparseMatrix& matrixP, const SparseMatrix& matrixA,
    const std::vector<c_float>& vectorL, const std::vector<c_float>& vectorU) {
    OSQPData osqpData;
    osqpData.n = matrixA.columnCount();
    osqpData.m = matrixA.rowCount();

    if (matrixP.columnCount() != osqpData.n
        || vectorL.size() != osqpData.m
        || vectorU.size() != osqpData.m) {
        throw UnexpectedException();
    }

    CompressedSparseColumnMatrix cscP(matrixP);
    osqpData.P = cscP.getCSCPtr();

    CompressedSparseColumnMatrix cscA(matrixA);
    osqpData.A = cscA.getCSCPtr();

    // q is not needed so a zero-filled array is used
    std::vector<c_float> vectorQ(static_cast<unsigned int>(osqpData.n), 0.0);
    osqpData.q = vectorQ.data();

    osqpData.l = const_cast<c_float*>(vectorL.data());
    osqpData.u = const_cast<c_float*>(vectorU.data());

    return OSQPSession(osqpData, osqp_settings).callOSQPSolve();
}
