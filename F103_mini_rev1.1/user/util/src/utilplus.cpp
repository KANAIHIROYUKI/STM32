#include "util.hpp"
#define _USE_MATH_DEFINES //Å@ÉŒÇóòópâ¬î\Ç…Ç∑ÇÈ
#include <math.h>

Coord::Coord(Coord const &coord){
	cartesianXData=coord.cartesianXData;
	cartesianYData=coord.cartesianYData;
	cartesianZData=coord.cartesianZData;
	eularXData=coord.eularXData;
	eularYData=coord.eularYData;
	eularZData=coord.eularZData;
}

Coord::Coord(float cx,float cy,float cz){
	cartesianXData=cx;
	cartesianYData=cy;
	cartesianZData=cz;
	eularXData=0;
	eularYData=0;
	eularZData=0;
}

Coord::Coord(float cx,float cy,float cz,float ex,float ey,float ez){
	cartesianXData=cx;
	cartesianYData=cy;
	cartesianZData=cz;
	eularXData=ex;
	eularYData=ey;
	eularZData=ez;
}

void Coord::cartesian(float cx,float cy,float cz){
	cartesianXData=cx;
	cartesianYData=cy;
	cartesianZData=cz;
}

void Coord::cartesianX(float x){
	cartesianXData=x;
}

void Coord::cartesianY(float y){
	cartesianYData=y;
}


void Coord::cartesianZ(float z){
	cartesianZData=z;
}

float Coord::cartesianX(){
	return cartesianXData;
}

float Coord::cartesianY(){
	return cartesianYData;
}

float Coord::cartesianZ(){
	return cartesianZData;
}

void Coord::cartesianXAdd(float x){
	cartesianX(cartesianX()+x);
}

void Coord::cartesianYAdd(float y){
	cartesianY(cartesianY()+y);
}

void Coord::cartesianZAdd(float z){
	cartesianZ(cartesianZ()+z);
}

void Coord::polar(float radius,float theta,float phi){
	float xy=0;
	if(theta==0){
		cartesianXData=0;
		cartesianYData=0;
		cartesianZData=radius;
		return;
	}
	cartesianZData=radius*cosf(theta);
	xy=radius*sinf(theta);
	cartesianXData=xy*cosf(phi);
	cartesianYData=xy*sinf(phi);
}

float Coord::polarRadius(){
	float radius;
	//    float theta;
	//    float phi;
	radius=sqrtf(powf(cartesianXData,2)+powf(cartesianYData,2)+powf(cartesianZData,2));
	//    phi=atan2f(cartesianYData,cartesianXData);
	//    if(radius==0) theta=0;
	//    else theta=acosf(cartesianZData/radius);
	return radius;
}

float Coord::polarTheta(){
	float radius;
	float theta;
	//float phi;
	radius=sqrtf(powf(cartesianXData,2)+powf(cartesianYData,2)+powf(cartesianZData,2));
	//phi=atan2f(cartesianYData,cartesianXData);
	if(radius==0) theta=0;
	else theta=acosf(cartesianZData/radius);
	return theta;
}

float Coord::polarPhi(){
//	float radius;
//	float theta;
	float phi;
	//    float radius;
	//float theta;
	//radius=sqrtf(powf(cartesianXData,2)+powf(cartesianYData,2)+powf(cartesianZData,2));
	phi=atan2f(cartesianYData,cartesianXData);
	//if(radius==0) theta=0;
	//else theta=acosf(cartesianZData/radius);
	return phi;
}

void Coord::polarRadius(float radiusData){
	float radius;
	float theta;
	float phi;
	radius=sqrtf(powf(cartesianXData,2)+powf(cartesianYData,2)+powf(cartesianZData,2));
	phi=atan2f(cartesianYData,cartesianXData);
	if(radius==0) theta=0;
	else theta=acosf(cartesianZData/radius);
	radius=radiusData;
	Coord::polar(radius,theta,phi);
}

void Coord::polarTheta(float thetaData){
	float radius;
	float theta;
	float phi;
	radius=sqrtf(powf(cartesianXData,2)+powf(cartesianYData,2)+powf(cartesianZData,2));
	phi=atan2f(cartesianYData,cartesianXData);
	if(radius==0) theta=0;
	else theta=acosf(cartesianZData/radius);
	theta=thetaData;
	Coord::polar(radius,theta,phi);
}

void Coord::polarPhi(float phiData){
	float radius;
	float theta;
	float phi;
	radius=sqrtf(powf(cartesianXData,2)+powf(cartesianYData,2)+powf(cartesianZData,2));
	phi=atan2f(cartesianYData,cartesianXData);
	if(radius==0) theta=0;
	else theta=acosf(cartesianZData/radius);
	phi=phiData;
	Coord::polar(radius,theta,phi);
}

void Coord::polarRadiusAdd(float radius){
	polarRadius(polarRadius()+radius);
}

void Coord::polarThetaAdd(float theta){
	polarTheta(polarTheta()+theta);
}

void Coord::polarPhiAdd(float phiArg){
	polarPhi(polarPhi()+phiArg);
}

void Coord::eular(float x,float y,float z){
	eularXData=x;
	eularYData=y;
	eularZData=z;
}

float Coord::eularX(){
	return eularXData;
}

float Coord::eularY(){
	return eularYData;
}

float Coord::eularZ(){
	return eularZData;
}

void Coord::eularX(float x){
	eularXData=x;
}

void Coord::eularY(float y){
	eularYData=y;
}

void Coord::eularZ(float z){
	eularZData=z;
}

void Coord::eularZAdd(float z){
	eularZData+=z;
}

float Coord::x(){
	return Coord::cartesianX();
}

float Coord::y(){
	return Coord::cartesianY();
}

float Coord::z(){
	return Coord::cartesianZ();
}

float Coord::radius(){
	return Coord::polarRadius();
}

float Coord::theta(){
	return Coord::polarTheta();
}

float Coord::phi(){
	return Coord::polarPhi();
}

float Coord::ex(){
	return Coord::eularX();
}

float Coord::ey(){
	return Coord::eularY();
}

float Coord::ez(){
	return Coord::eularZ();
}


float Coord::Reradius(float deg){
	float rad = deg * M_PI / 180.0;
	return rad;
}

void Coord::revolutionXY(float angle){
	float cartesianXSub = cartesianXData;
	float cartesianYSub = cartesianYData;
	eularZData = eularZData + angle;
	//	int a = cosf(angle);
	//	std::cout << std::endl << " angle : " << a << std::endl;
	cartesianXData = cartesianXSub * cosf(angle) + (-1 * cartesianYSub * sinf(angle));
	cartesianYData = cartesianXSub * sinf(angle) + cartesianYSub * cosf(angle);
}

void Coord::revolutionYZ(float angle){
	float cartesianYSub = cartesianYData;
	float cartesianZSub = cartesianZData;
	eularXData = eularXData + angle;
	cartesianYData = cartesianYSub * cosf(angle) - cartesianZSub * sinf(angle);
	cartesianZData = cartesianYSub * sinf(angle) + cartesianZSub * cosf(angle);
}

void Coord::revolutionZX(float angle){
	float cartesianXSub = cartesianXData;
	float cartesianZSub = cartesianZData;
	eularYData = eularYData + angle;
	cartesianZData = cartesianZSub * cosf(angle) - cartesianXSub * sinf(angle);
	cartesianXData = cartesianZSub * sinf(angle) + cartesianXSub * cosf(angle);
}

void Coord::revolutionX(float angle){
	float cartesianXSub = cartesianXData;
	float cartesianYSub = cartesianYData;
	float cartesianZSub = cartesianZData;
	eularXData = eularXData + angle;
	cartesianXData = cartesianXSub;
	cartesianYData = cartesianYSub * cosf(angle) - cartesianZSub * sinf(angle);
	cartesianZData = cartesianYSub * sinf(angle) + cartesianZSub * cosf(angle);
}

void Coord::revolutionY(float angle){
	float cartesianXSub = cartesianXData;
	float cartesianYSub = cartesianYData;
	float cartesianZSub = cartesianZData;
	eularYData = eularYData + angle;
	cartesianXData = cartesianXSub * cosf(angle) + cartesianZSub * sinf(angle);
	cartesianYData = cartesianYSub;
	cartesianZData = (-1 * cartesianYSub * sinf(angle)) + cartesianZSub * cosf(angle);
}

void Coord::revolutionZ(float angle){
	float cartesianXSub = cartesianXData;
	float cartesianYSub = cartesianYData;
	float cartesianZSub = cartesianZData;
	eularZData = eularZData + angle;
	cartesianXData = cartesianXSub * cosf(angle) - cartesianYSub * sinf(angle);
	cartesianYData = cartesianXSub * sinf(angle) + cartesianYSub * cosf(angle);
	cartesianZData = cartesianZSub;
}

void Coord::revolutionXYCartesian(float angle){
    float cartesianXSub = cartesianXData;
    float cartesianYSub = cartesianYData;
    //eularZData = eularZData + angle;
    //	int a = cosf(angle);
    //	std::cout << std::endl << " angle : " << a << std::endl;
    cartesianXData = cartesianXSub * cosf(angle) + (-1 * cartesianYSub * sinf(angle));
    cartesianYData = cartesianXSub * sinf(angle) + cartesianYSub * cosf(angle);
}

void Coord::revolutionYZCartesian(float angle){
    float cartesianYSub = cartesianYData;
    float cartesianZSub = cartesianZData;
    //eularXData = eularXData + angle;
    cartesianYData = cartesianYSub * cosf(angle) - cartesianZSub * sinf(angle);
    cartesianZData = cartesianYSub * sinf(angle) + cartesianZSub * cosf(angle);
}

void Coord::revolutionZXCartesian(float angle){
    float cartesianXSub = cartesianXData;
    float cartesianZSub = cartesianZData;
    //eularYData = eularYData + angle;
    cartesianZData = cartesianZSub * cosf(angle) - cartesianXSub * sinf(angle);
    cartesianXData = cartesianZSub * sinf(angle) + cartesianXSub * cosf(angle);
}

void Coord::revolutionXCartesian(float angle){
    float cartesianXSub = cartesianXData;
    float cartesianYSub = cartesianYData;
    float cartesianZSub = cartesianZData;
    //eularXData = eularXData + angle;
    cartesianXData = cartesianXSub;
    cartesianYData = cartesianYSub * cosf(angle) - cartesianZSub * sinf(angle);
    cartesianZData = cartesianYSub * sinf(angle) + cartesianZSub * cosf(angle);
}

void Coord::revolutionYCartesian(float angle){
    float cartesianXSub = cartesianXData;
    float cartesianYSub = cartesianYData;
    float cartesianZSub = cartesianZData;
    //eularYData = eularYData + angle;
    cartesianXData = cartesianXSub * cosf(angle) + cartesianZSub * sinf(angle);
    cartesianYData = cartesianYSub;
    cartesianZData = (-1 * cartesianYSub * sinf(angle)) + cartesianZSub * cosf(angle);
}

void Coord::revolutionZCartesian(float angle){
    float cartesianXSub = cartesianXData;
    float cartesianYSub = cartesianYData;
    float cartesianZSub = cartesianZData;
    //eularZData = eularZData + angle;
    cartesianXData = cartesianXSub * cosf(angle) - cartesianYSub * sinf(angle);
    cartesianYData = cartesianXSub * sinf(angle) + cartesianYSub * cosf(angle);
    cartesianZData = cartesianZSub;
}

void Coord::revolutionEuler(float a, float b, float c){
	float cartesianXSub = cartesianXData;
	float cartesianYSub = cartesianYData;
	float cartesianZSub = cartesianZData;
	cartesianXData = cartesianXSub * (cos(a) * cosf(c) - sinf(a) * cosf(b) * sinf(c)) + cartesianYSub * (cos(a) * sinf(c) + sinf(a) * cosf(b) * cosf(c)) + cartesianZSub * (sin(a) * sinf(b));
	cartesianYData = cartesianXSub * (-1 * sinf(a) * cosf(c) - cosf(a) * cosf(b) * sinf(c)) + cartesianYData * (-1 * sinf(a) * sinf(c) + cosf(a) * cosf(b) * cosf(c)) + cartesianZData * (cos(a) * sinf(b));
	cartesianZData = cartesianXSub * (sin(b) * sinf(c)) + cartesianYSub * (-1 * sinf(b) * cosf(c)) + cartesianZSub * cosf(b);

}

void Coord::revolutionEulerinverse(float a, float b, float c){
	float cartesianXSub = cartesianXData;
	float cartesianYSub = cartesianYData;
	float cartesianZSub = cartesianZData;
	cartesianXData = cartesianXSub * (cos(a) * cosf(c) - sinf(a) * cosf(b) * sinf(c)) + cartesianYSub * (-1 * sinf(a) * cosf(c) + -1 * cosf(a) * cosf(b) * sinf(c)) + cartesianZSub * (sin(b) * sinf(c));
	cartesianYData = cartesianXSub * (cos(a) * sinf(c) + sinf(a) * cosf(b) * cosf(c)) + cartesianYSub * (-1 * sinf(a) * sinf(c) + cosf(a) * cosf(b) * cosf(c)) + cartesianZSub * (-1 * sinf(b) * cosf(c));
	cartesianZData = cartesianXSub * (sin(a) * sinf(b)) + cartesianYSub * (cos(a) * sinf(b)) + cartesianZSub * cosf(b);

}

Coord operator+(Coord coord1,Coord coord2){
	return Coord(coord1.x()+coord2.x(),coord1.y()+coord2.y(),coord1.z()+coord2.z(),coord1.ex()+coord2.ex(),coord1.ey()+coord2.ey(),coord1.ez()+coord2.ez());
}

Coord operator-(Coord coord1,Coord coord2){
	return Coord(coord1.x()-coord2.x(),coord1.y()-coord2.y(),coord1.z()-coord2.z(),coord1.ex()-coord2.ex(),coord1.ey()-coord2.ey(),coord1.ez()-coord2.ez());
}

Coord operator*(float value,Coord coord2){
	return Coord(coord2.x()*value,coord2.y()*value,coord2.z()*value,coord2.ex()*value,coord2.ey()*value,coord2.ez()*value);
}

Coord operator*(Coord coord1,float value){
	return Coord(coord1.x()*value,coord1.y()*value,coord1.z()*value,coord1.ex()*value,coord1.ey()*value,coord1.ez()*value);
}

Coord operator/(Coord coord1,float value){
	return Coord(coord1.x()/value,coord1.y()/value,coord1.z()/value,coord1.ex()/value,coord1.ey()/value,coord1.ez()/value);
}

Coord operator/(float value,Coord coord2){
	return Coord(value/coord2.x(),value/coord2.y(),value/coord2.z(),value/coord2.ex(),value/coord2.ey(),value/coord2.ez());
}


int sign(int val)
{
	if(val>=0) return 1;
	else return -1;
}

int sign(float val)
{
	if(val>=0.0) return 1;
	else return -1;
}

int quadraticEquationSolver(float &x1_arg,float &x1i_arg,float &x2_arg,float &x2i_arg
                            ,float a,float b,float c){//a*x^2+b*x^1+c=0
    std::complex<float> x1,x2;

    //calc
    typedef std::complex<float> compf;
    x1=(-b+ std::sqrt(compf(b*b-4*a*c,0.0f)) )/(2.0f*a);
    x2=(-b- std::sqrt(compf(b*b-4*a*c,0.0f)) )/(2.0f*a);

    //assignment
    x1_arg=x1.real();
    x1i_arg=x1.imag();
    x2_arg=x2.real();
    x2i_arg=x2.imag();

    //return number of complex roots
    if(x1i_arg!=0.0&&x2i_arg!=0.0)return 2;
    else return 0;
}

int quadraticEquationSolver(std::complex<float> &x1,std::complex<float> &x2
                            ,float a,float b,float c){//a*x^2+b*x^1+c=0
    //calc
    typedef std::complex<float> compf;
    x1=(-b+ std::sqrt(compf(b*b-4*a*c,0.0f)) )/(2.0f*a);
    x2=(-b- std::sqrt(compf(b*b-4*a*c,0.0f)) )/(2.0f*a);

    //return number of complex roots
    if(x1.imag()!=0.0f&&x2.imag()!=0.0f)return 2;
    else return 0;
}


int cubicEquationSolver(std::complex<float> &x1,std::complex<float> &x2,std::complex<float> &x3
                        ,float a,float b,float c,float d){
    typedef std::complex<float> compf;
    //a*x^3+c*x^2+c*x^1+d=0

    //x^3+A*x^2+B*x+C=0
    float A=b/a;
    float B=c/a;
    float C=d/a;

    //X=x+A/3
    //assign x=X-A/3
    //X^3+p*X+q=0
    //X^3 + (B - A^2 /3)*X + (2*A^3)/27 - (B*A)/3 + C = 0
    float p=(B-A*A/3.0f);
    float q=(2.0f*A*A*A)/27.0f-B*A/3.0f+C;

    compf w=compf(-1.0f/2.0f,sqrtf(3.0f)/2.0f);

    compf uCube=-q/2.0f+sqrt(compf(q*q/4.0f+p*p*p/27.0f,0.0f));
    compf u=pow(uCube,1.0f/3.0f);

    compf u1=u;
    compf u2=u*w;
    compf u3=u*w*w;

    compf v1,v2,v3;

    //Is assign 0 correct?
    if(u1==compf(0.0f,0.0f))v1=compf(0.0f,0.0f);
    else v1=-p/(3.0f*u1);
    if(u2==compf(0.0f,0.0f))v2=compf(0.0f,0.0f);
    else v2=-p/(3.0f*u2);
    if(u3==compf(0.0f,0.0f))v3=compf(0.0f,0.0f);
    else v3=-p/(3.0f*u3);

    compf X1=u1+v1;
    compf X2=u2+v2;
    compf X3=u3+v3;

    x1=X1-A/3.0f;
    x2=X2-A/3.0f;
    x3=X3-A/3.0f;

    int i=0;//return number of complex roots
    if(x1.imag()!=0) i++;
    if(x2.imag()!=0) i++;
    if(x3.imag()!=0) i++;

    return i;
}

int cubicEquationSolver(float &x1r,float &x1i,float &x2r,float &x2i,float &x3r,float &x3i
                        ,float a,float b,float c,float d){
    typedef std::complex<float> compf;
    compf x1,x2,x3;

    int i=cubicEquationSolver(x1,x2,x3,a,b,c,d);

    x1r=x1.real();
    x2r=x2.real();
    x3r=x3.real();
    x1i=x1.imag();
    x2i=x2.imag();
    x3i=x3.imag();

    return i;
}
