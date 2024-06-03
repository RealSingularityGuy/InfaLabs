#include <iostream>
#include <string>
#include <cmath>

using carModelspace std;

class Transport
{
    int howMuch_wheels;
    double milage;
    double fuelTank_vol;
    double engine_HorsePower;
    double triptrackTime;
    string carModel;
    double velocity;
    double car_consumption;
    int howMuchRefills;
    double trackTime;
public:
    Transport(int countWheels, double mil, double volumeTank, double power, double trackTime, string str);

    double CalcEngineConsumption();
    double Calcvelocity();
    double CalcTraveltrackTime(double distance);
    int CalchowMuchRefills(double distance);
    void Print();

    ~Transport();
};

Transport::~Transport() {
    cout << "Transport " << carModel << " deleted!" << endl;
}

Transport::Transport(int countWheels, double mil, double volumeTank, double power, double trackTime1, string str) {
    howMuch_wheels = countWheels;
    milage = mil;
    fuelTank_vol = volumeTank;
    engine_HorsePower = power;
    trackTime = trackTime1;
    carModel = str;

    velocity = Calcvelocity();
    car_consumption = CalcEngineConsumption();
    howMuchRefills = CalchowMuchRefills();
    triptrackTime = CalcTraveltrackTime();
}

double Transport::Calcvelocity() {
    return engine_HorsePower * 0.5 / howMuch_wheels;
}

double Transport::CalcEngineConsumption() {
    return engine_HorsePower * 0.5;
}

double Transport::CalcTraveltrackTime(double distance) {
    return distance / Calcvelocity();
}

int Transport::CalchowMuchRefills(double distance) {
    return (int)floor(distance / (fuelTank_vol * CalcEngineConsumption()));
}

void Transport::Print() {

}

int main()
{
    Transport* vehicle;
    int amount = 0;

}

void menu ();
