#include <iostream>
#include <ceres/ceres.h>

class testClass{
    public:
        testClass(double bias):bias(bias){};

        template<typename T> bool operator()(const T* x, T* residual)const
        {
            residual[0] = T(10)- x[0] + T(bias);
            residual[1] = T(10)- x[0] + T(bias);
            residual[2] = T(10)- x[0] + T(bias);
            residual[3] = T(10)- x[0] + T(bias);
    
            // Return
            return true;
        }
    private:
        double bias;
};

int main(int argc, char** argv){


    // Copy initial ceres conditions
    double x(0);
    double x_ini(x);

    // Create problem and add block
    ceres::Problem problem;
    ceres::CostFunction* cost_function = new ceres::AutoDiffCostFunction<testClass, 3, 1>(
            new testClass(2));
    problem.AddResidualBlock(cost_function, NULL, &x);

    // Solve
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    options.max_num_iterations = 10;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    // Display results
    std::cout << summary.FullReport() << std::endl;
    std::cout<<x_ini<<" -> "<<x<<std::endl;

    return 0;
}
