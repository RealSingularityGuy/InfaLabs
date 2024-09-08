#include "cmath"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

int quantity = 0; //исходный размер массива
double InputProve(double var);
int InputProve(int var);

class vehicle {
private:
    double tank_Capacity; //обЪём бака в литрах
    double velocity; //скорость км/ч km/h
    double engine_consumption; //расход 1 л / 100 км
    double engine_Power; //мощность двигателя в л.с.
    int wheels_Count;

public:
    double mileage; //пробег в км
    double number_of_refuels; //количество дозаправок
    string name;
    double travel_Time; //путевое время в часах
    inline double Calculatevelocity() {
        return fabs(sqrt(engine_Power) * ((70 / double(wheels_Count)) - 2.5));
    }
    inline double CalculateIntake() {
        return fabs(pow(engine_Power, 1 / 3) + sqrt(engine_Power) - 6.25);
    }
    double CalculateRefuel(double raceLength) {
        double number_of_refuels = (raceLength * (engine_consumption / 100)) / tank_Capacity;
        if (number_of_refuels > 1)
            return ceil(number_of_refuels);
        else
            return floor(number_of_refuels);
    };
    inline double CalculateRacetravel_Time(double raceLength) {
        return (raceLength / velocity);
    };
    vehicle() {
        name = "ADDVEHICLE";
        mileage = travel_Time = number_of_refuels = 0;
    }
    vehicle(string vehicle_name, double tank, double power, int wheels) {
        mileage = travel_Time = number_of_refuels = 0;
        cout << "\nName of the car: ";
        cin >> vehicle_name;
        SetName(vehicle_name);
        cout << "Number of wheels: ";
        wheels = InputProve(wheels);

        while ((wheels < 2)) {
            cout << "Uncorrect number of wheels! try again:" << endl;
            wheels = InputProve(wheels);
        }

        Setwheels_Count(wheels);
        cout << "Power of the engine (HP): ";
        power = InputProve(power);
        SetPowerEngine(power);
        cout << "Tank capacity (l): ";
        tank = InputProve(tank);
        Settank_Capacity(tank);
        velocity = Calculatevelocity();
        engine_consumption = CalculateIntake();
        cout << "\nCar added successfully!" << endl;
    }
    void Settank_Capacity(double tank) { tank_Capacity = tank; }
    void SetPowerEngine(double power) { engine_Power = power; }
    void SetName(string vehicle_name) { name = vehicle_name; }
    void Setwheels_Count(int wheels) { wheels_Count = wheels; }
    ~vehicle() { cout << "Destruction of " << name << endl; }
    void Output();
    void travel_TimeDisplay();
};

int Menu(int& flag);
void Clean(int var = 1);
void Addvehicle(vehicle*& adres, int& quantity, vehicle cars);
vehicle* RatingResults(vehicle*& adres, int quantity);
void OutputResults(vehicle*& adres, int quantity);


int main() {
    Clean();
    vehicle* adres = nullptr;
    double trackLen = 0;
    int flag = 1;
    int rez = 0;
    while (flag == 1) {
        int choice = 10;
        choice = Menu(rez);
        if ((choice > 5) || (choice < 0)) {
            cout << "\nNot right, try again!\n";
        }
        switch (choice) {
        case (0): {
            cout << "Confirm exit?\n1-YES\n0-NO\n";
            int exit = 2;
            exit = InputProve(exit);
            if (exit == 1) {
                flag = 0;
                break;
            }
            else {
                Clean();
            }
            break;
        }
                delete[] adres;
                break;
        case (1): {
            Clean();
            string vehicle_name = "";
            double tank = 0;
            double power = 0;
            int wheels_Count = 0;
            vehicle cars(vehicle_name, tank, power, wheels_Count);
            Clean();
            Addvehicle(adres, quantity, cars);
            rez = 0;
            break;
        }
        case (2): {
            Clean();
            for (int i = 0; i < quantity; i++) {
                cout << adres[i].name << endl;
                adres[i].Output();
                cout << endl;
            }
            break;
        }
        case (3): {
            Clean();
            cout << "Enter the length of the track (km): ";
            trackLen = InputProve(trackLen);
            rez = 0;
            break;
        }
        case (4): {
            Clean();
            if (trackLen == 0) {
                cout << "You haven't entered the length of the track!\n";
                rez = 0;
                break;
            }
            else {
                for (int i = 0; i < quantity; i++) {
                    adres[i].travel_Time = adres[i].CalculateRacetravel_Time(trackLen);
                    adres[i].mileage = trackLen;
                    adres[i].number_of_refuels = adres[i].CalculateRefuel(trackLen);
                }
            }
            break;
        }
        case (5): {
            Clean();
            OutputResults(*&adres, quantity);
            break;
        }
        default:
            Clean();
            break;
        }
    }

    return 0;
}

void Clean(int var) {
    for (int i = 0; i < 3; i = i + 1) {
        cout << "\n";
    }
}
int Menu(int& rez) {
    cout << "\n############MAIN MENU############\n";
    cout << "1 - Add new vehicle\n";
    cout << "2 - Info about vehicle\n";
    cout << "3 - Input distance\n";
    cout << "4 - Calculate the track\n";
    if (rez == 1)
        cout << "5 - Show results of the last race\n";
    cout << "0 - Quit the program\n";
    int choice = 0;
    choice = InputProve(choice);
    if (choice == 4) {
        rez = 1;
    }
    return choice;
}
void Addvehicle(vehicle*& adres, int& quantity, vehicle cars) {
    vehicle* tempArray;
    if (adres != nullptr) {
        tempArray = new vehicle[quantity + 1];
        for (int i = 0; i < quantity; i++) {
            tempArray[i] = adres[i];
        }
        delete[] adres;
    }
    else {
        tempArray = new vehicle[1];
    }
    tempArray[quantity] = cars;
    adres = tempArray;
    quantity++;
}

vehicle* RatingResults(vehicle*& adres, int quantity) {
    vehicle* results = new vehicle[quantity];
    vehicle* rez_copy = new vehicle[1];
    for (int i = 0; i < quantity; i++) {
        results[i] = adres[i];
    }
    for (int i = 0; i < quantity; i++) {
        for (int j = 0; j < quantity; j++) {
            if ((results[i].travel_Time - results[j].travel_Time < 0) &&
                (results[i].number_of_refuels - results[i].number_of_refuels <= 0)) {
                rez_copy[0] = results[i];
                results[i] = results[j];
                results[j] = rez_copy[0];
            }
        }
    }
    delete[] rez_copy;
    return results;
}

void OutputResults(vehicle*& adres, int quantity) {
    vehicle* results = RatingResults(adres, quantity);
    for (int i = 0; i < quantity; i++) {
        cout << endl << results[i].name << endl;
        results[i].travel_TimeDisplay();
        cout << "Refuel travel_Times: " << int(ceil(results[i].number_of_refuels)) << endl << endl;
    }
    delete[] results;
}

void vehicle::Output() {
    cout << "Number of wheels: " << wheels_Count << ";\n"
        << "Power of engine: " << engine_Power << " HP;\n"
        << "velocity of vehicle: " << velocity << " km/h;\n"
        << "Engine consumprion: " << engine_consumption << " l/100km;\n"
        << "Mileage: " << mileage << " km;" << endl;
}
double InputProve(double var) {
    cin >> var;
    if (cin.fail() || var <= 0) {
        while (!(cin >> var) || var <= 0) {
            cout << "Incorrect, try again, please\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return var;
}
int InputProve(int var) {
    cin >> var;
    if (cin.fail() || var < 0) {
        while (!(cin >> var) || var < 0) {
            cout << "Incorrect, try again, please\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return var;
}
void vehicle::travel_TimeDisplay() {
    double t = this->travel_Time;
    int hours = static_cast<int>(t);
    double cur_travel_Time = (t - hours) * 60;
    int minutes = static_cast<int>((cur_travel_Time));
    int seconds = static_cast<int>((cur_travel_Time - minutes) * 60);
    cout << "travel_Time: " << hours << ":" << minutes << ":" << seconds << endl;
}
