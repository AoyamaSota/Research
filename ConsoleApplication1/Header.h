
cv::Mat bin_img;
CvMat *h;
PointGrayCamera camera;
CvMoments moment;
cv::Point2d img_center;

CvPoint2D32f hand_center;
int i = 0, sum;
int flag;
cv::Mat mat2;
cv::Mat gray_img;
cv::Mat bininv_img, trunc_img, tozero_img, tozeroinv_img;
int counter = 0;
cv::Mat dst_img;
cv::Mat src_img;
cv::Mat projection;
//cv::Mat
cv::Mat background;
cv::Mat pattern;
cv::Mat_<double> homo;
//int count=0;
int j = 0;
int sign = 1;
double h_tmp[16] = { 0 };
cv::Size patch2(1280, 960);
cv::Size patch(640, 480);
DWORD startTime;
double timer;
int fps = 0;
int move = 0;
int condition;
// アニメーションの周期
#define CYCLE 1
// テクスチャオブジェクト
static GLuint texname;

std::vector<cv::Point2d> center;
std::vector<double> v;
cv::Point2d posi;
cv::Point2d hand;


// asa
cv::Mat tmpmat;
cv::Mat tmpmat2;
std::vector<cv::Point2d> tnkpoint;

// カルマンフィルタ
CvKalman *kalman = cvCreateKalman(4, 2);

//初期化
void init(void);

//ウインドウのリサイズ
static void resize(int w, int h);

// テクスチャ生成
//bool  loadTextureFromMat1(cv::Mat& image, GLuint* texture);

//アニメーション
static void idle(void);

//CVで処理した画像をGLのテクスチャに変換
static void getTexture(void);

//GLで表示
static void display(void);

// OpenGL の初期化
static void glInit(void);

static void keyboard(unsigned char key, int x, int y);