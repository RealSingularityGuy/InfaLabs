#include "cmath"
#include <iostream>
#include <string>
#include <limits>
#include <cmath>

using namespace std;
int quantity = 0; //исходный размер массива
double InputProve(double var);
int InputProve(int var);

class wheel {
  double current_mileage;
  int status; //Наличие повреждений или их отсутствие у колеса (1 или 0 соотв.)
public:
  int check_status(double mileage, double velocity);
  wheel() {status=0;current_mileage=0;};
  double def_wheel(double mileage, double velocity) {
    current_mileage = mileage;
    status = check_status(mileage,velocity);

    return current_mileage;
  }
  virtual void output();
  int get_status() { return status; }
};

class engine {
protected:
  double engine_Power; //мощность двигателя в лошадиных силах л.с./hp
  double engine_Consumption; //расход автомобиля 1 л / 100 км
public:
 inline double consumption_Calculation() {return fabs(pow(engine_Power, 1 / 3) + sqrt(engine_Power) - 6.25);}
 engine(){engine_Consumption=0; engine_Power=0;};
  void def_engine(double power);
  virtual void output();
};

class fuel_system {
protected:
  double curr_Fuel_Level; //текущее количество топлива, литры;
public:
  double tank_Capacity; //обЪём бака в литрах
  void calculate_cur_fuel(double engine_Consumption, double mileage, double number_of_refuels);
  fuel_system(){tank_Capacity=0;curr_Fuel_Level=0;};
  void def_fuel_system(double capacity, double engine_Consumption, double mileage);
  virtual void output();
};

class vehicle : public engine, public fuel_system {
private:
  int number_of_wheels;
  double velocity; //скорость км/ч km/h
  double travelTime;    //время пути hour
  int damaged_wheels = 0;
  double mileage; //пробег km
  double number_of_refuels; //количество дозаправок

public:
  wheel *ptr_wheel;
  string name;
  vehicle() {
    name = "ADDVEHICLE";
    travelTime = mileage = number_of_refuels = damaged_wheels = 0;
    ptr_wheel = 0;
  }
  vehicle(string vehicle_name, int wheels) {
    mileage =damaged_wheels = 0;
    setName(vehicle_name);
    setnumber_of_wheels(wheels);
    ptr_wheel = new wheel[wheels];
    for (int i = 0; i < wheels; i++) {
      ptr_wheel[i] = wheel();
    }
    def_fuel_system(0, engine_Consumption, mileage);
    def_engine(0);
    calculatevelocity();
  }

  void calculatevelocity();
  void setName(string vehicle_name) { name = vehicle_name; }
  void setnumber_of_wheels(int wheels) { number_of_wheels = wheels; }
  void setMileage(double tracklen) { mileage=tracklen; }
  ~vehicle() { cout << "Destruction of " << name << endl; }
  inline void calculateRacetravelTime(double raceLength) {
    travelTime = (raceLength / velocity);
  };

  double get_travelTime(){return travelTime;};
  double get_number_of_refuels(){return number_of_refuels;};
 double get_velocity(){return velocity;};
  void calculateRefuel(double raceLength);
  void travelTime_display();
  int getnumber_of_wheels() { return number_of_wheels; }
  void number_of_damaged_wheels();
  void output();
  friend vehicle *RatingResults(int quantity);
  friend void outputResults(vehicle *&car,int quantity);
  friend ostream &operator<<(ostream &stream, const vehicle &obj) {
    return stream << "Number of wheels: " << obj.number_of_wheels << ";\n"
                  << "Damaged wheels: " << obj.damaged_wheels << ";\n"
                  << "velocity: " << obj.velocity << " km/h;\n"
                  << "Mileage: " << obj.mileage << " km;\n"
                  << "fuel capacity: " << obj.tank_Capacity << "l;\n"
                  << "current fuel: " << obj.curr_Fuel_Level << "l\n"
                  << "engine power: " << obj.engine_Power << " HP;\n"
                  << "engine intake: " << obj.engine_Consumption << " l/100km;" << endl;
  }
};

int menu(int &flag);
void clean(int var = 1);
void create_vehicle(vehicle *&address, int &quantity, vehicle cars);



int main() {
  clean();
  vehicle *address = nullptr;
  double trackLen = 0;
  int flag = 1;
  int rez = 0;
  while (flag == 1) {
    int choice = 10;
    choice = menu(rez);
    if ((choice > 5) || (choice < 0)) {
      cout << "\nError, try another number!\n";
    }
    switch (choice) {
    case (0): {
      cout << "Confirm exit?\n1-yes\n0-go back\n";
      int exit = 2;
      exit = InputProve(exit);
      if (exit == 1) {
        flag = 0;
        break;
      } else {
        clean();
      }
      break;
    }
      delete[] address;
      for (int i = 0; i < quantity; i++) {
        delete[] address[i].ptr_wheel;
      }
      break;
    case (1): {
      clean();
      string vehicle_name = "";
      cout << "\nName of the vehicle: ";
      cin >> vehicle_name;
      int number_of_wheels = 0;
      cout << "Number of wheels: ";
      number_of_wheels = InputProve(number_of_wheels);
      while ((number_of_wheels < 2)) {
        cout << "Uncorrect number of wheels! try again:" << endl;
        number_of_wheels = InputProve(number_of_wheels);
      }
      vehicle cars(vehicle_name, number_of_wheels);
      clean();
      create_vehicle(address, quantity, cars);
      rez = 0;
      break;
    }
    case (2): {
      clean();
      for (int i = 0; i < quantity; i++) {
        cout << address[i].name << endl;
        address[i].output();
        cout << "__OR__" << endl;
        cout << address[i];
      }
      break;
    }
    case (3): {
      clean();
      cout << "Enter the length of the track (km): ";
      trackLen = InputProve(trackLen);
      rez = 0;
      break;
    }
    case (4): {
      clean();
      if (trackLen == 0) {
        cout << "You haven't entered the length of the track!\n";
        rez = 0;
        break;
      }
      else {
        for (int i = 0; i < quantity; i++) {
          address[i].calculateRacetravelTime(trackLen);
          address[i].setMileage(trackLen);
          for (int j = 0; j < address[i].getnumber_of_wheels(); j++) {
          address[i].ptr_wheel[j].def_wheel(trackLen,address[i].get_velocity());
          }
          address[i].number_of_damaged_wheels();
          address[i].calculateRefuel(trackLen);
          address[i].calculate_cur_fuel(address[i].consumption_Calculation(), trackLen,address[i].get_number_of_refuels());
          address[i].calculatevelocity();
        }
      }
      break;
    }
    case (5): {
      clean();
      outputResults(address,quantity);
      break;
    }
    default:
      clean();
      break;
    }
  }

  return 0;
}

void clean(int var) {
  for (int i = 0; i < 3; i = i + 1) {
    cout << "\n";
  }
}

int menu(int &rez) {
  cout << "\nEnter a number to choose action:\n";
  cout << "1 Add a new vehicle\n";
  cout << "2 Output the information about vehicle\n";
  cout << "3 Enter a lenght of the track\n";
  cout << "4 Calculate rout\n";
  cout << "0 Exit\n";

  if (rez == 1)
    cout << "5 Results of the race\n";
  int choice = 0;
  choice = InputProve(choice);
  if (choice == 4) {
    rez = 1;
  }
  return choice;
}

void create_vehicle(vehicle *&address, int &quantity, vehicle cars) {
  vehicle *tempArray;
  if (address != nullptr) {
    tempArray = new vehicle[quantity + 1];
    for (int i = 0; i < quantity; i++) {
      tempArray[i] = address[i];
    }
    delete[] address;
  } else {
    tempArray = new vehicle[1];
  }
  tempArray[quantity] = cars;
  address = tempArray;
  quantity++;
}

vehicle *RatingResults(vehicle *&address, int quantity) {
  vehicle *results = new vehicle[quantity];
  vehicle *rez_copy = new vehicle[1];
  for (int i = 0; i < quantity; i++) {
    results[i] = address[i];
  }
  for (int i = 0; i < quantity; i++) {
    for (int j = 0; j < quantity; j++) {
      if ((results[i].get_travelTime()- results[j].get_travelTime() < 0) &&
          (results[i].get_number_of_refuels() - results[i].get_number_of_refuels() <= 0)) {
        rez_copy[0] = results[i];
        results[i] = results[j];
        results[j] = rez_copy[0];
      }
    }
  }
  delete[] rez_copy;
  return results;
}

void outputResults(vehicle *&address, int quantity) {
  vehicle *results = RatingResults(address,quantity);
  for (int i = 0; i < quantity; i++) {
    cout << results[i].name << endl;
    results[i].travelTime_display();
    cout << "Refuel travelTimes: " << int(results[i].number_of_refuels) << endl;
  }
  delete[] results;
}

double InputProve(double var) {
  cin >> var;
  if (cin.fail() || var <= 0) {
    while (!(cin >> var) || var <= 0) {
      cout << "Not correct, try again\n";
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
      cout << "Not correct, try again\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }
  return var;
}

int wheel::check_status(double mileage, double velocity) {
  double ratio = (1/(mileage*sqrt(velocity)));
  if (ratio>=0.5)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

void wheel ::output() {
  if (status == 1) {
    cout << "damaged" << endl;
  } else {
    cout << "not damaged" << endl;
  }
}

void engine ::output() {
  cout << "engine power: " << engine_Power << " HP;" << endl;
  cout << "engine intake: " << engine_Consumption << " l/100km;" << endl;
}

void engine::def_engine(double power) {
  cout << "Engine power in HP: ";
  power = InputProve(power);
  engine_Power = power;
  engine_Consumption = consumption_Calculation();
}

void fuel_system ::calculate_cur_fuel(double engine_Consumption, double mileage,double number_of_refuels) { //расчёт текущего обЪёма топлива
  if (number_of_refuels == 0) {
    curr_Fuel_Level=double(tank_Capacity - ((engine_Consumption / 100) * mileage));
  } else {
    curr_Fuel_Level=(((number_of_refuels)*tank_Capacity) - ((engine_Consumption / 100) * mileage));
  }
}

void fuel_system ::def_fuel_system(double capacity, double engine_Consumption,double mileage) {
  cout << "Fuel capacity: ";
  capacity = InputProve(capacity);
  tank_Capacity = capacity;
  curr_Fuel_Level = capacity;
}

void fuel_system ::output() {
  cout << "Fuel capacity: " << tank_Capacity << "l" << endl;
  cout << "Current fuel: " << curr_Fuel_Level << "l" << endl;
}

void vehicle::output() {
  cout << "Number of wheels: " << number_of_wheels << ";\n"
       << "Damaged wheels: " << damaged_wheels << ";\n"
       << "Velocity: " << velocity << " km/h;\n"
       << "Mileage: " << mileage << " km;" << endl;
  engine::output();
  fuel_system::output();
}

void vehicle ::travelTime_display() {
  double t = this->travelTime;
  int hours = int(t);
  double cur_travelTime = (t - hours) * 60;
  int minutes = int(cur_travelTime);
  int seconds = int((cur_travelTime - minutes) * 60);
  cout << "Travel Time: " << hours << ":" << minutes << ":" << seconds << endl;
}

void vehicle ::calculatevelocity() {
  if (damaged_wheels == 0) {
velocity= double(fabs(sqrt(engine_Power) * (70.0 / double(number_of_wheels) - 2.5) * (curr_Fuel_Level / 100.0)));
 }
 else
 {
   velocity = (fabs(sqrt(engine_Power) * (70.0 / double(number_of_wheels) - 2.5) * (curr_Fuel_Level / 100.0))/(pow(2., double(damaged_wheels))));
     }
  }


void vehicle ::number_of_damaged_wheels() {
  int count = 0;
  for (int i = 0; i < number_of_wheels; i++) {
    if (ptr_wheel[i].get_status() == 1) {
      count++;
    }
  }
  damaged_wheels = count;
}

void vehicle ::calculateRefuel(double raceLength) {
  double Refuel = (raceLength * (engine_Consumption / 100)) / tank_Capacity;
  if (Refuel > 1)
    this->number_of_refuels= ceil(Refuel);
  else
    this->number_of_refuels= floor(Refuel);
}
