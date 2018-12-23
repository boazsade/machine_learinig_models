#ifndef _LIBSVM_H
#define _LIBSVM_H

#define LIBSVM_VERSION 318


extern int libsvm_version;

struct svm_node
{
    constexpr svm_node() = default;
    constexpr svm_node(int i, double v) :
        index{i}, value{v}
    {
    }

    int index = -1;
    double value = 0;
};

struct svm_problem
{
    svm_problem() = default;
    svm_problem(int len, double* yv, svm_node** xv);

    int l = -1;
    double *y = nullptr;
    svm_node **x = nullptr;
};

enum SVM_TYPE { 
    C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR 
};	
enum KERNEL_TYPE { 
    LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED 
};
struct svm_parameter
{
    constexpr svm_parameter() = default;
    SVM_TYPE svm_type = C_SVC;
    KERNEL_TYPE kernel_type = LINEAR;
    int degree = 0;	/* for poly */
    double gamma = 0;	/* for poly/rbf/sigmoid */
    double coef0 = 0;

	/* these are for training only */
    double cache_size = 0; /* in MB */
    double eps = 0;	/* stopping criteria */
    double C = 0; 	/* for C_SVC, EPSILON_SVR and NU_SVR */
    int nr_weight = 0;		/* for C_SVC */
    int *weight_label = nullptr;	/* for C_SVC */
    double* weight = nullptr;		/* for C_SVC */
    double nu = 0;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
    double p = 0;	/* for EPSILON_SVR */
    bool shrinking = false;	/* use the shrinking heuristics */
    bool  probability = false; /* do probability estimates */
};

//
// svm_model
// 
struct svm_model
{
    constexpr svm_model() = default;
    svm_parameter param;	        /* parameter */
    int nr_class = 2;		/* number of classes, = 2 in regression/one class svm */
    int l = 0;			/* total #SV */
    svm_node **SV = nullptr;	/* SVs (SV[l]) */
    double **sv_coef = nullptr;	/* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
    double *rho = nullptr;		/* constants in decision functions (rho[k*(k-1)/2]) */
    double *probA = nullptr;	/* pariwise probability information */
    double *probB = nullptr;
    int *sv_indices = nullptr;        /* sv_indices[0,...,nSV-1] are values in [1,...,num_traning_data] to indicate SVs in the training set */

	/* for classification only */

    int *label = nullptr;	/* label of each class (label[k]) */
    int *nSV = nullptr;     /* number of SVs for each class (nSV[k]) */
				/* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
	/* XXX */
    bool free_sv = false;	/* 1 if svm_model is created by svm_load_model*/
				/* 0 if svm_model is created by svm_train */
};

struct svm_model *svm_train(const svm_problem *prob, const svm_parameter *param);
void svm_cross_validation(const svm_problem *prob, const svm_parameter *param, int nr_fold, double *target);

int svm_save_model(const char *model_file_name, const svm_model *model);
struct svm_model *svm_load_model(const char *model_file_name);

int svm_get_svm_type(const svm_model *model);
int svm_get_nr_class(const svm_model *model);
void svm_get_labels(const svm_model *model, int *label);
void svm_get_sv_indices(const svm_model *model, int *sv_indices);
int svm_get_nr_sv(const svm_model *model);
double svm_get_svr_probability(const svm_model *model);

double svm_predict_values(const svm_model *model, const svm_node *x, double* dec_values);
double svm_predict(const svm_model *model, const svm_node *x);
double svm_predict_probability(const svm_model *model, /*const*/ svm_node *x, double* prob_estimates);

void svm_free_model_content(svm_model *model_ptr);
void svm_free_and_destroy_model(svm_model **model_ptr_ptr);
void svm_destroy_param(svm_parameter *param);

const char *svm_check_parameter(const svm_problem *prob, const svm_parameter *param);
int svm_check_probability_model(const svm_model *model);

void svm_set_print_string_function(void (*print_func)(const char *));


#endif /* _LIBSVM_H */
