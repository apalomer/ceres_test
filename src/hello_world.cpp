#include "ceres/ceres.h"
#include <type_traits>
using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;

class ceresCallback : public ceres::IterationCallback {
   public:
        ceresCallback(ceres::Problem* problem):problem(problem){}
        ~ceresCallback(){}

        ceres::CallbackReturnType operator()(const ceres::IterationSummary& summary){
            std::vector<double*> parameters;
            problem->GetParameterBlocks(&parameters);
            for (int i=0;i<parameters.size();i++){
                int k = problem->ParameterBlockSize(parameters[i]);
                for (int j=0;j<k;j++)
                    std::cout<<" x["<<i<<","<<j<<"] = "<<parameters[i][j];
            }
            std::cout<<std::endl;
            return ceres::SOLVER_CONTINUE;
        }
   private:
       ceres::Problem* problem;
};

struct CostFunctor {
   template <typename T>
   bool operator()(const T* const x, T* residual) const {
       std::cout<<"x[0]: "<<x[0]<<std::endl;
       if (std::is_same<T,ceres::Jet<double,2> >::value){
           std::cout<<"is jet"<<std::endl;
           ceres::Jet<double,2> j;
       }
       std::cout<<"x[0]: "<<x[0]<<std::endl;
       std::cout<<"x[1]: "<<x[1]<<std::endl;
     residual[0] = T(10.0) - x[0] - x[1];
     return true;
   }
};

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);

    // The variable to solve for with its initial value.
    double initial_x[] = {5,0};
    double x[] = {initial_x[0],initial_x[1]};

    // Build the problem.
    Problem problem;

    // Set up the only cost function (also known as residual). This uses
    // auto-differentiation to obtain the derivative (jacobian).
    CostFunction* cost_function =
    new AutoDiffCostFunction<CostFunctor, 1, 2>(new CostFunctor);
    problem.AddResidualBlock(cost_function, NULL, x);

    // Run the solver!
    Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    ceresCallback* cb = new ceresCallback(&problem);
    options.callbacks.push_back(cb);
    options.update_state_every_iteration = true;
    Solver::Summary summary;
    Solve(options, &problem, &summary);

    std::cout << summary.FullReport() << "\n";
    std::cout << "x : " << initial_x[0] <<","<< initial_x[1]
        << " -> " << x[0] <<","<< x[1] << "\n";

    return 0;
}
