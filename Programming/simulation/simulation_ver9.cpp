#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <map>
#include <vector>
#include <cstring>
#include <istream>
#include <complex>
#include <ctime>
using namespace std;
const double max_acceleration = 7.0;//最大减速度（的绝对值） 
double dealt_time; // 已经处理的时间长度 
struct Point
{
    double x, y;
    Point (double x = 0, double y = 0):x(x),y(y)
    {
    }
};
typedef Point Vector;

struct Car
{
    Point center;
    Vector Direction;
    int rd;//road 
    double acceleration=0;//加速度 
    Car(Point center=Point(0,0),Vector Direction=Point(0,0)):center(center),Direction(Direction)
    {
    }
};

Vector operator + (Vector A, Vector B)
{
    return Vector(A.x+B.x,A.y+B.y);
}
Vector operator - (Vector A, Vector B)
{
    return Vector(A.x-B.x,A.y-B.y);
}
Vector operator * (Vector A, double p)
{
    return Vector(A.x*p,A.y*p);
}
Vector operator / (Vector A, double p)
{
    return Vector(A.x/p,A.y/p);
}
bool operator < (const Point &a, const Point &b)
{
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}
//double operator % (double a, double b) { // 两个正小数取余运算 
//	double ans = 0;
//	while(b*ans<=a) ans = ans + 1;
//	return (a - b * (ans-1)); 
//}
double mod (double a, double b) {
	double ans = 0.0;
	while (b * ans <= a) ans = ans + 1;
	return a - (ans - 1) * b;
}

double Dot(Vector A, Vector B)
{
    return A.x*B.x + A.y*B.y;
}
double Length(Vector A)
{
    return sqrt(Dot(A,A));
}
double Angle(Vector A, Vector B)
{
    return acos(Dot(A,B))/Length(A)/Length(B);
}
double Cross(Vector A, Vector B)
{
    return A.x*B.y - A.y*B.x;
}
double Area2(Point A, Point B, Point C)
{
    return Cross(B-A, C-A);
}
//cross+area2 求面积 
Vector Rotate(Vector A, double rad)
{
    return Vector(A.x*cos(rad)-A.y*sin(rad), A.x*sin(rad)+A.y*cos(rad));
}
//旋转向量 
// please make sure its not a zero vector!
Vector Normal(Vector A)//旋转90°并单位化 
{
    double L = Length(A);
    return Vector(-A.y/L,A.x/L);
}
Vector Format(const Vector &A)//换算单位向量 
{
    double L=Length(A);
    return Vector(A.x/L,A.y/L);
}

const double eps = 1e-10;//精度 
int dcmp(double x)
{
    if(fabs(x) < eps)//fabs=abs 
        return 0;
    else
        return x < 0 ? -1 : 1;
}

bool operator == (const Point &a, const Point &b)
{
    return dcmp(a.x-b.x)==0 && dcmp(a.y-b.y)==0;
}
Point read_point()//duliu input point
{
    double x, y;
    cin >> x >> y;
    return Vector(x, y);
}
//??? 
Point GetLineIntersection (Point P, Vector v, Point Q, Vector w)//解析式 
{
    Vector u=P-Q;
    double t = Cross(w,u)/Cross(v,w);
    return P+v*t;
}
//F*ck CCF
double DistanceToLine (Point P, Point A, Point B)//点到直线距离 
{
    Vector v1 = B-A,v2=P-A;
    return fabs(Cross(v1,v2))/Length(v1);
}
double DistanceToSegment (Point P, Point A, Point B)//点到线段距离 
{
    if (A==B)
        return Length(P-A);
    Vector v1=B-A,v2=P-A,v3=P-B;
    if(dcmp(Dot(v1,v2))<0)
        return Length(v2);
    else if(dcmp(Dot(v1,v3))>0)
        return Length(v3);
    else
        return fabs(Cross(v1,v2))/Length(v1);
}

Point GetLineProjection(const Point& P, const Point& A, const Point& B)//直线投影 
{
    Vector v = B - A;
    return A + v * (Dot(v, P - A) / Dot(v, v));
}

bool SegmentProperIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2)//交点 
{
    double c1 = Cross(a2 - a1, b1 - a1), c2 = Cross(a2 - a1, b2 - a1),
    c3 = Cross(b2 - b1, a1 - b1), c4 = Cross(b2 - b1, a2 - b1);
    return dcmp(c1) * dcmp(c2) < 0 && dcmp(c3) * dcmp(c4) < 0;
}
//FUCKCCF 
bool isPointOnSegment(const Point& p, const Point& a1, const Point& a2)//判断点在线段上 
{
    return dcmp(Cross(a1 - p, a2 - p)) == 0 && dcmp(Dot(a1 - p, a2 - p)) < 0;
}
double PolygonArea(Point* p, int n)//求多边形面积，*p为端点数组 
{
    double area = 0;
    for (int i = 1; i < n - 1; i++)
        area += Cross(p[i] - p[0], p[i + 1] - p[0]);
    return area / 2;
}

struct Circle//圆 
{
    Point c;
    double r;
    Circle(Point c = Point(), double r = 0) :c(c), r(r) {}
    inline Point point(double a)
    {
        return Point(c.x + cos(a) * r, c.y + sin(a) * r);
    }
};
Circle read_circle()//duliu input 
{
    Circle C;
    scanf("%lf%lf%lf", &C.c.x, &C.c.y, &C.r);
    return C;
}
//ccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsbccfsb 
int GetLineCircleIntersection(const Point& A, const Point& B, const Circle& C, vector<Point>& sol)//直线和圆的交点数量 
{
    double d = DistanceToLine(C.c, A, B);
    int mode = dcmp(d - C.r);
    if (mode > 0) return 0;
    Point P = GetLineProjection(C.c, A, B);
    if (mode == 0)
    {
        sol.push_back(P);
        return 1;
    }
    double dist = sqrt(C.r * C.r - d * d);
    Vector v = Format(B - A);
    sol.push_back(P - v * dist);
    sol.push_back(P + v * dist);
    return 2;
}

int GetCircleCircleIntersection (Circle C1, Circle C2, vector<Point>& sol)//两个圆的交点数量
{
    if (C1.r < C2.r) swap(C1, C2);
    double D = Length(C1.c - C2.c);
    if (dcmp(D) == 0)
        return dcmp(C1.r - C2.r) == 0 ? -1 : 0;
    if (dcmp(C1.r + C2.r - D) < 0) return 0;
    if (dcmp(fabs(C1.r - C2.r) - D) > 0) return 0;

    double d1 = ((C1.r * C1.r - C2.r * C2.r) / D + D) / 2;
    double x = sqrt(C1.r * C1.r - d1 * d1);
    Point O = C1.c + Format(C2.c - C1.c) * d1;
    Point P1 = O + Normal(O - C2.c) * x, P2 = O - Normal(O - C2.c) * x;
    sol.push_back(P1);
    if (P1 == P2) return 1;
    sol.push_back(P2);
    return 2;
}
//��￡ 
inline int GetTangents (const Point P, const Circle C, vector<Point>& v)//切线 
{
    Vector u = C.c - P;
    double dist = Length(u);
    int mode = dcmp(dist - C.r);
    if (mode < 0) return 0;
    if (mode == 0)
    {
        v.push_back(P + Normal(u));
        return 1;
    }
    double x = sqrt(dist * dist - C.r * C.r);
    Circle C2(P, x);
    return GetCircleCircleIntersection(C, C2, v);
}

double min_decelarate_dis(double v, double a)
{
	return v * v / a / 2;
}

double site_x[20] =
    {
        0.00,
        151.17,
        371.11,
        0.00,
        151.17,
        371.11,
        0.00,
        151.17,
        371.11,
        0.00,
        151.17,
        371.11,
        0.00,
        151.17,
        251.26,
        0.00,
        151.17
    };
double site_y[20] =
    {
        295.19,
        295.19 ,
        295.19 ,
        224.14 ,
        224.14 ,
        224.14 ,
        170.97 ,
        170.97 ,
        170.97 ,
        107.43 ,
        107.43 ,
        107.43 ,
        48.89 ,
        48.89 ,
        48.89 ,
        0.00 ,
        0.00
    };
const int LEN_OF_SITES = 17;
// from A to Q, 17 points in total
//道士打告 
struct Road
{
    int start;
    int end;
    Car first_car;
	double lim_v;
	double width;
    Road(int start, int end):start(start),end(end)
    {
    }
};
//shabiccf 
Point sites[20];//道路交点 
Car cs[1005];//cs=cars 

Road roads[33] =
    {
        Road(2, 1),
        Road(1, 0),
        Road(3, 4),
        Road(4, 5),
        Road(6, 7),
        Road(7, 8),
        Road(8, 7),
        Road(7, 6),
        Road(9, 10),
        Road(10, 11),
        Road(11, 10),
        Road(10, 9),
        Road(12, 13),
        Road(13, 14),
        Road(15, 16),
        Road(0, 3),
        Road(3, 6),
        Road(1, 4),
        Road(4, 7),
        Road(7, 10),
        Road(10, 13),
        Road(13, 16),
        Road(16, 13),
        Road(13, 10),
        Road(10, 7),
        Road(7, 4),
        Road(4, 1),
        Road(15, 12),
        Road(2, 5),
        Road(5, 8),
        Road(8, 11),
        Road(16, 14),
        Road(14, 11)
    };

const double UPPER = 295.59;
const double LOWER = 0.00;
const double LEFT = 0.00;
const double RIGHT = 371.11;
const double SEP = 151.17;
const double SEP2 = 107.43;

const double CAR_WIDTH = 1.6;
const double CAR_LENGTH = 4.8;

const Vector TO_UP = Vector(0, 1);
const Vector TO_DOWN = Vector(0, -1);
const Vector TO_LEFT = Vector(-1, 0);
const Vector TO_RIGHT = Vector(1, 0);

bool is_same_dir(Vector a, Vector b)//判断两个向量是否同向 
{
    if(Dot(a,b)==Length(a)*Length(b))
    {
        return true;
    }
    return false;
}

bool is_out(Car vehicle)
{
    // bool c1 = (vehicle.center.y == UPPER && vehicle.center.x >= LEFT && vehicle.center.x <= RIGHT);
    // bool c2 = (vehicle.center.y == LOWER && vehicle.center.x >= LEFT && vehicle.center.x <= SEP);
    // bool c3 = (vehicle.center.x >= SEP && vehicle.center.x <= RIGHT && vehicle.center.y < 0.49 * vehicle.center.x - 73.8);
    // bool c4 = (vehicle.center.x == LEFT && vehicle.center.y >= LOWER && vehicle.center.y <= UPPER);
    // bool c5 = (vehicle.center.x == RIGHT && vehicle.center.y >= SEP2 && vehicle.center.y <= UPPER);
    bool c1 = (vehicle.center.y >= UPPER && is_same_dir(vehicle.Direction,TO_UP));
    bool c2 = (vehicle.center.y <= LOWER && is_same_dir(vehicle.Direction,TO_DOWN));
    bool c3 = (vehicle.center.x >= SEP && vehicle.center.x <= RIGHT && vehicle.center.y < 0.49 * vehicle.center.x - 73.8 && is_same_dir(vehicle.Direction,TO_RIGHT));
    bool c4 = (vehicle.center.x <= LEFT && is_same_dir(vehicle.Direction,TO_LEFT));
    bool c5 = (vehicle.center.x >= RIGHT && is_same_dir(vehicle.Direction,TO_RIGHT));
    return (c1 || c2 || c3 || c4 || c5);
}
int num_of_car;
#define nc num_of_car
/*
灏嗘瘡涓矾娈电殑杞﹁締浠庡墠鍒板悗渚濇鎺掑簭锛岀粍鎴愪竴涓槦鍒�
闃熷垪涓殑鍏冪礌涓� Car, 姣忎釜鍏冪礌闇�瑕佹湁涓�涓墠椹� prev 涓庡悗缁� next, 杩樻湁 Car 鏈韩鐨勫睘鎬�, 浠ュ強鍒拌揪涓嬩竴涓矾鍙ｇ殑棰勮鑺辫垂鏃堕棿 t
姣忛殧1绉掓洿鏂颁竴娆℃暟鎹�
*/
double point_dist(Point a,Point b)
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

struct cross
{
	int outrd[6];
	int roadnumber;//道路数量 
	double red, green; // 此处 red 定义为 编号0/2 的出边的红灯
	double initstatus; // initstatus 定义为 编号0/2 的出边第一次绿灯转红灯的时候距离初始时间的时刻 
}crosses[44];

int get_num(int rd_number)
{
	int cnt=0;
	for(int i=1;i<=nc;i++)
	{
		if(isPointOnSegment(cs[i].center,sites[roads[i].start],sites[roads[i].end])) cnt++;
	}
	return cnt;
}// 杈撳嚭璇ヨ矾娈垫湁澶氬皯杈嗚溅

bool red_or_green (double time, int cross, int sta) { // 0=red 1=green
	double cyctime = mod(time + crosses[cross].initstatus, crosses[cross].red + crosses[cross].green);
	bool red_02 = (cyctime < crosses[cross].red);
	if ((red_02 + (sta % 2)) == 1) return 0;
	return 1;
}

double car_turning[1009],decelerate_time[1009],accelerate_time[1009];//记录每辆车还需要多少时间转弯 以及还要多少时间减速到0 
Car get_first_start(int rd_number)
{
	Car nearest;
	if(!get_num(rd_number)) return nearest;
	double minn=998244353.0;
	int mink=114514;
	Vector dir=sites[roads[rd_number].start]-sites[roads[rd_number].end];
	bool flag=0;
	for(int i=1;i<=nc;i++)
	{
		if(isPointOnSegment(cs[i].center,sites[roads[i].start],sites[roads[i].end]))
		{
			if(minn>point_dist(cs[i].center,sites[roads[rd_number].start])&&Dot(dir,cs[i].Direction)>0)
			{
				mink=i;
				minn=point_dist(cs[i].center,sites[roads[rd_number].start]);
				nearest=cs[i]; 
				flag=1;
			}
		}
	}
	return nearest;
}// 鎵惧埌璇ヨ矾娈垫渶闈犲墠鐨勪竴杈嗚溅

Car get_first_end(int rd_number) {
	Car nearest;
	if(!get_num(rd_number)) {
		return nearest;
	}
	double minn = 998244353.0;
	int mink = 114514;
	Point dir = sites[roads[rd_number].start] - sites[roads[rd_number].end];
	bool flag = 0;
	for(int i = 1; i <= nc; i++) {
		if (isPointOnSegment(cs[i].center, sites[roads[i].start], sites[roads[i].end])) {
			if (minn > point_dist(cs[i].center, sites[roads[rd_number].end]) && Dot(dir,cs[i].Direction) < 0) {
				mink = i;
				minn = point_dist(cs[i].center, sites[roads[rd_number].end]);
				nearest = cs[i];
			}
		}
	}
	return nearest;
} // 鎵惧埌璇ヨ矾娈垫渶闈犲墠鐨勪竴杈嗚溅
#define crossroads GetLineIntersection(roads[i].start, roads[i].end - roads[i].start, roads[j].start, roads[j].end - roads[j].start)
bool get_to_intersection(Car cr) {
	for(int i = 0; i < 33; i++) { 
		for(int j = i + 1; j < 33; j++) {
			if(crossroads == cr.center) return 1; 
		}
	} 
	return 0;
}

//TODO: 乱码 
// 鍒ゆ柇杞﹁締鏄惁鍒拌揪璺彛
// 0.5s update 涓�娆�
//roads 0~16横向 17~30纵向 3132斜向 
struct turn {
	int to_num;
	double turn_time;
};

turn rand_turn(int crossroad_number, int status) { // rand-status=0 掉头 rand-status=1 右转 rand-status=2 直行 rand-status=3 左转 
	srand(time(0));
	int rand_turn = rand() % crosses[crossroad_number].roadnumber;
	turn to_road;
	to_road.to_num = crosses[crossroad_number].outrd[rand_turn];
	int turnstatus = (rand_turn + 4 - status) % 4;
	if(turnstatus < 2) {
		to_road.turn_time = 5.0;
	} else if (turnstatus == 2) {
		to_road.turn_time = 1.5;
	} else {
		to_road.turn_time = 0.5;
	} 
	return to_road;
} //右转5s 直行1.5s 左转0.5s 掉头5s

double max_upspeed(double a, double s, double maxv, int cross) {
	/*
	思路：先判断先加速到最大速度再匀速能否过红灯
	如果不行，再判断何时减速 
	*/
	double acc_dis = maxv * maxv / a / 2;
	double acc_time = maxv / a;
	double other_time = (s - acc_dis) / maxv;
	double red_time = (crosses[cross].red + crosses[cross].green) - mod(dealt_time + crosses[cross].initstatus, crosses[cross].red + crosses[cross].green);
	if (acc_time + other_time <= red_time) {//能通过
		return maxv;
	}
	double maxv_s = maxv / 2 * (maxv / a + maxv / max_acceleration);
	if (maxv_s <= s) return maxv;
	return sqrt(s * 2 / (1 / a + 1 / max_acceleration));
} 

void update() { // 0.5second
	//TODO: how to judge whether a car can pass a traffic light
	//      how to update the variable 'done'
	bool done = 0;
	for (int i = 1; i <= 100; i++) {
		printf("%d号车: x=%lf, y=%lf, on road number%d, speed=%lf\n", i, cs[i].center.x, cs[i].center.y, cs[i].rd, Length(cs[i].Direction));
	}
	//TODO: 乱码 
    // 浠� get_first(...) 浠庡墠寰�鍚庢洿鏂�
        // 鍋囧鑷繁涓嶆槸 prev
            // 鍋囧 prev 闇�瑕佸噺閫�
                // 鏍规嵁鍏紡1璁＄畻璇ヨ溅鐨勭浉搴旀渶澶у噺閫熷害锛屽苟鎶婃墍鏈変笌璇ヨ溅鏈夊叧鐨勭姸鎬佹洿鏂�
            // 鍋囧 prev 姝ｅ父琛岄┒
                // 璁＄畻 t 骞跺垽鏂嚜宸辨槸鍚﹂渶瑕佸噺閫�
                    // 鍋囧闇�瑕佸噺閫�, 鏇存柊鍑忛�熷害涓哄叕寮�2
        // 鍋囧鑷繁鏄� prev
            // 闅忔満鏁扮湅杞粈涔堝集锛堝湪鍏佽鐨勮溅鍚戣寖鍥村唴锛屼笖涓嶅噯鎺夊ご锛�
                // 灏忚浆寮洿鎺ヨ浆锛屾坊鍔犲埌鍙︿竴涓矾娈甸槦鍒楃殑鏈熬
                // 鍒ゆ柇鑷繁鏄惁闇�瑕佸噺閫�
                    // 闇�瑕�
                        // 浠ユ渶澶у噺閫熷害鍑忛�熷嵆鍙�
                        // 绛夊緟缁跨伅
                        // 缁跨伅浜悗浠ユ渶澶у姞閫熷害杩涜涓嬮潰鐨勬搷浣�
                    // 涓嶉渶瑕侊細姝ｅ父鏇存柊
                        // 鍋囧鍦� 1s 鍐呰兘鍒拌揪璺彛(get_to_intersection), 姝ｅ父杞集骞惰繘鍏ュ彟涓�鏉¤矾娈甸槦鍒楁湯灏�
	dealt_time = dealt_time + 0.5;
	int crsrd_num, sta;
	for(int i = 0; i < 33; i++) { // across
		for(int j = i + 1; j < 33; j++) { // not across
			if (!isPointOnSegment(crossroads, roads[i].start, roads[i].end) || !isPointOnSegment(crossroads, roads[j].start, roads[j].end)) continue; // 不存在的交点（交点存在于线段外） 
			Point dir = sites[roads[i].start]-sites[roads[i].end];
			if(crossroads == roads[i].start) {
				for(int k=1;k<=nc;k++) {
					if(isPointOnSegment(cs[k].center,sites[roads[i].start],sites[roads[i].end])&&Dot(dir,cs[k].Direction)>0&&car_turning[k]==0) { // 准备转弯
						if(get_to_intersection(cs[k])) {
							if (accelerate_time[i] > 0) {
								cs[i].Direction = cs[i].Direction * (Length(cs[i].Direction) + 0.5 * max_acceleration) / Length(cs[i].Direction);
								accelerate_time[i] = accelerate_time[i] - 0.5; 
							}
							for(int z = 0; z < 20; z++) {
								if (sites[z] == crossroads) {
									crsrd_num = z;
									break;
								}
							}
							for(int z=0;z<4;z++) {
								if (i == crosses[crsrd_num].outrd[z]) {
									sta = z;
									break;
								}
							}
							turn Turning = rand_turn(crsrd_num, sta);
							car_turning[k] = Turning.turn_time;
							cs[k].rd = Turning.to_num;
							if (crossroads == sites[roads[cs[k].rd].start]) {
								cs[k].Direction = sites[roads[cs[k].rd].end] - sites[roads[cs[k].rd].start];
							} else {
								cs[k].Direction = sites[roads[cs[k].rd].start] - sites[roads[cs[k].rd].end];
							}
						}
					}
					else if (isPointOnSegment(cs[i].center, sites[roads[i].start], sites[roads[i].end]) && Dot(dir, cs[i].Direction) > 0 && car_turning[i] != 0) {
						car_turning[i] -= 0.5;
						if (car_turning[i] <= 0) {
							accelerate_time[i] = max_upspeed(max_acceleration, Length(roads[cs[i].rd].start - roads[cs[i].rd].end), roads[cs[i].rd].lim_v, crsrd_num);
						}
					}
					else {
						if ( /*能通过*/ 1) { // 红绿灯能通过 条件占位 
							cs[k].center = cs[k].center + (cs[k].Direction / 2); 
						}
						else { // 无法通过，减速 
							if (decelerate_time[k]>0) {
								cs[k].Direction = cs[k].Direction * (Length(cs[k].Direction) + max_acceleration) / Length(cs[k].Direction);
							}
							if (point_dist(cs[k].center, sites[roads[i].start]) <= min_decelarate_dis(Length(cs[k].Direction), max_acceleration)) {
								decelerate_time[k] = Length(cs[k].Direction) / max_acceleration;
							}
						} 
					}
				}
			}
		}
	}
}
void RandCar() {
	srand(time(0));
	for(int i = 1; i <= 100; i++) {
		int rand_road = rand() % 33;
		cs[i].rd = rand_road;
		Road r = roads[rand_road];
		int langda = rand() % 10000;
		double proportion = (double)langda / 10000;
		cs[i].center = r.start * proportion + r.end * (1 - proportion);
		cs[i].Direction = double(rand() % 70000) / 10000;
	}
}

/*
寮曞叆鏅鸿兘璺伅鍚庯細
1. 鍒硅溅璺濈绾︿负 16m锛屽嵆璺彛蹇呴』 lidar 鍏ㄨ鐩�
2. 瀵逛簬璺涓病鏈� lidar 瑕嗙洊鐨勮溅杈嗭紝鎸夌収鍖�閫熻椹�
3. wifi 鍒嗛厤鎸夌収浜屽垎鍥惧尮閰嶇畻娉曟潵
*/











int main()
{
    // Point test=Point(1.5,2.5);
    // cout << dcmp(-0.1) << endl;
    // cout << dcmp(1e-11) << endl;
    // cout << test.x << " " << test.y << endl;
    // Point new_test = read_point();
    // cout << new_test.x << endl;
	RandCar();
    for(int i = 0; i < LEN_OF_SITES;i++) {
        sites[i] = Point(site_x[i], site_y[i]);
    }

    // for (int i = 0; i < LEN_OF_SITES;i++)
    // {
    //     cout <<char(65+i)<<"\t"<< sites[i].x << "\t" << sites[i].y << endl;
    // }
	for(int i = 0; i < 3; i++) {
		roads[i].lim_v = 70.0 / 3.6;
	}
	for(int i = 3; i < 33; i++) {
		roads[i].lim_v = 50.0 / 3.6;
	}
    Car test(sites[14], Point(3.3,0));
    // cout << test.center.x << endl
    //      << test.center.y << endl
    //      << test.Direction.x << endl
    //      << test.Direction.y << endl;
    // cout << is_out(test);
    cout << is_out(test) << endl;
    // cout << is_same_dir(Vector(1, 0), Vector(9, 0));

	crosses[1].outrd[0] = 15;
	crosses[1].outrd[1] = 1;
	crosses[1].roadnumber = 2;
	crosses[2].outrd[0] = 1;
	crosses[2].outrd[1] = 17;
	crosses[2].outrd[2] = 0;
	crosses[2].roadnumber = 3;
	crosses[3].outrd[0] = 0;
	crosses[3].outrd[1] = 28;
	crosses[3].roadnumber = 2;
	crosses[4].outrd[0] = 16;
	crosses[4].outrd[1] = 2;
	crosses[4].outrd[3] = 15;
	crosses[4].roadnumber = 3;
	crosses[5].outrd[0] = 17;
	crosses[5].outrd[1] = 2;
	crosses[5].outrd[2] = 18;
	crosses[5].outrd[3] = 3;
	crosses[5].roadnumber = 4;
	crosses[6].outrd[0] = 28;
	crosses[6].outrd[1] = 3;
	crosses[6].outrd[2] = 29;
	crosses[6].roadnumber = 3;
	crosses[7].outrd[0] = 7;
	crosses[7].outrd[1] = 16;
	crosses[7].roadnumber = 2;
	crosses[8].outrd[0] = 18;
	crosses[8].outrd[1] = 7;
	crosses[8].outrd[2] = 19;
	crosses[8].outrd[3] = 5;
	crosses[8].roadnumber = 4;
	crosses[9].outrd[0] = 28;
	crosses[9].outrd[1] = 5;
	crosses[9].outrd[2] = 30;
	crosses[9].roadnumber = 3;
	crosses[10].outrd[0] = 8;
	crosses[10].roadnumber = 1;
	crosses[11].outrd[0] = 19;
	crosses[11].outrd[1] = 8;
	crosses[11].outrd[2] = 20;
	crosses[11].outrd[3] = 9;
	crosses[11].roadnumber = 4;
	crosses[12].outrd[0] = 10;
	crosses[12].roadnumber = 1;
	crosses[13].outrd[0] = 12;
	crosses[13].roadnumber = 1;
	crosses[14].outrd[0] = 21;
	crosses[14].outrd[1] = 13;
	crosses[14].outrd[2] = 20;
	crosses[14].roadnumber = 3;
	crosses[15].outrd[0] = 32;
	crosses[15].roadnumber = 1;
	crosses[16].outrd[0] = 14;
	crosses[16].outrd[1] = 27;
	crosses[16].roadnumber = 2;
	crosses[17].outrd[0] = 31;
	crosses[17].outrd[1] = 22;
	crosses[17].roadnumber = 2;
	for (int i = 1; i < 18; i++) {
		crosses[i].initstatus = rand() % 100; // 随机取每个路口的初始值
		// 每个路口的循环长度均为 100s? 
	}
	roads[0].width = roads[1].width = 40;
	roads[2].width = roads[3].width = 10;
	for(int i = 4; i < 8; i++) {
		roads[i].width = 17;
	}
	for(int i = 8; i < 12; i++) {
		roads[i].width = 23;
	}
	roads[12].width = roads[13].width = roads[27].width = 8;
	roads[14].width = roads[31].width = roads[32].width = 14;
	roads[15].width = roads[16].width = roads[28].width = roads[29].width = roads[30].width = 15;
	for(int i = 17; i < 27; i++) {
		roads[i].width = 17;
	}
	for(int i = 1; i <= 17; i++) {
		crosses[i].green = 100 * (roads[crosses[i].outrd[0]].width) / (roads[crosses[i].outrd[0]].width + roads[crosses[i].outrd[1]].width);
		crosses[i].red = 100 - crosses[i].green;
	}
	update();
    return 0;
}
