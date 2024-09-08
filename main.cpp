#include <iostream>
#include "cmath"
#include <limits>
#include <string>
#include <ctime>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iomanip>

using namespace std;
double Refuel_travel_Time = 0.0003;//около 1 сек
double Change_travel_Time = 0.008;//около 30 секунд
double dt = 0.0003;
double InputProve(double var);
int InputProve(int var);
int menu(int &rez);
void clean(int var = 1);

class wheel {
  double current_Distance;
  int status; //повреждено или исправно 1/0
public:
  int check_status(double mileage, double velocity);
  wheel() {status=0;current_Distance=0;};
  virtual ~wheel(){};
  void def_wheel(double mileage, double velocity);
  void output();
  int get_status() { return status; }
  void set_status(int status) {
    this->status = status;
  }
};

class engine {
protected:
  double engine_Power; //мощность двигателя в л.с.
  double engine_Consumption; //потребление двигателя литров/км l/100km
public:
 inline double calculateConsumption() {return fabs(pow(engine_Power, 1 / 3) + sqrt(engine_Power) - 6.25);}
 engine(){engine_Consumption=0;engine_Power=0;};
  void def_engine(double power);
};

class fuel_system {
protected:
  double fuel_Level; //текущее количество топлива
public:
  double tank_Capacity; //обЪём бака в литрах
  void calculate_cur_fuel(double engine_Consumption, double mileage);
  fuel_system(){tank_Capacity=0;fuel_Level=0;};
  void def_fuel_system();
};

class vehicle : public engine, public fuel_system {
private:
  int Nwheels;
  double velocity; //скорость км/ч km/h
  double travel_Time;    //время пути hour
  int damaged_wheels;
  double number_of_refuels; //количество дозаправок
  double mileage; //пробег в км
  double current_Distance;//текущее положение на круге km
  int current_Circles;
public:
  vector<wheel> vec_wheels;
  string name;
  double pit_stop_travel_Time;
  vehicle() {
    name = "ADDVEHICLE";
    travel_Time = current_Distance=mileage = number_of_refuels = pit_stop_travel_Time = 0;
    damaged_wheels=current_Circles=0;
  }
  vehicle(string vehicle_name, int wheels) {
    current_Distance=mileage=pit_stop_travel_Time=0;
    damaged_wheels=current_Circles=0;
    name=vehicle_name;
    Nwheels=wheels;
    for (int i = 0; i < wheels; i++) {
      wheel newWheel;
      vec_wheels.push_back(newWheel);
    }
    def_fuel_system();
    def_engine(0);
    calculatevelocity();
  }

  void calculatevelocity();
  void setMileage(double tracklen) { mileage=tracklen; }
  ~vehicle() { cout << "Destruction of " << name << endl; }
  inline void calculateRacetravel_Time(double raceLength) {
    travel_Time = (raceLength / velocity);}
   void reset();

  void total_travel_Time()
  {
    travel_Time=travel_Time+pit_stop_travel_Time;
  }

  int calc_circles(double trackLen)
  {
    if (current_Distance-trackLen>=0)
    {
      current_Circles=int(mileage/trackLen);
      current_Distance=0;
      return 1;
    }
    else {return 0;}
  }

  double wheels_change()
  {
    double travel_Time=damaged_wheels*Change_travel_Time;
    return travel_Time;
  }

  double refuel_travel_Time()
  {
    double travel_Time=((tank_Capacity-fuel_Level)*Refuel_travel_Time);
    return travel_Time;
  }

  double get_travel_Time(){return travel_Time;};
  void set_travel_Time(double travel_Time){this->travel_Time=travel_Time;};
  double get_number_of_refuels(){return number_of_refuels;};
  void plus_number_of_refuels(){number_of_refuels++;}
  double get_velocity(){return velocity;};
  int get_damaged_wheels(){return damaged_wheels;};
  double get_mileage(){return mileage;};
  void set_mileage(double len){this->mileage=len;};
  void mileage_plus(){this->mileage+=(velocity*dt);};
  double get_current_Distance(){return current_Distance;};
  void set_current_Distance(double len){this->current_Distance=len;};
  void current_Distance_plus(){this->current_Distance+=(velocity*dt);};
  int get_current_Circles(){return current_Circles;};
  void set_current_Circles(int circles){this->current_Circles=circles;};
  void current_Circles_plus(){this->current_Circles++;}
  void calculateRefuel(double raceLength,int circles);
  void need_refuel(double tracklen);
  void need_change();
  void travel_Time_display();
  int getNwheels() { return Nwheels; }
  double get_fuel_Level() { return fuel_Level; }
  void number_of_damaged_wheels();
  void output();
  friend vehicle *RatingResults(int qty);
  friend void outputResults(vector <vehicle> car,int qty);

  friend ostream &operator<<(ostream &stream, const vehicle &obj) {
    return stream <<"\n"<<"Name: "<<obj.name<<"\n"
      << "Number of wheels: " << obj.Nwheels << ";\n"
                  << "Damaged wheels: " << obj.damaged_wheels << ";\n"
                  << "Velocity: " << obj.velocity << " km/h;\n"
                  << "Mileage: " << obj.mileage << " km;\n"
                  << "Fuel capacity: " << obj.tank_Capacity << " l;\n"
                  << "Current fuel: " << obj.fuel_Level << " l\n"
                  << "Engine power: " << obj.engine_Power << " HP;\n"
                  << "Engine consumption: " << obj.engine_Consumption << " l/100km;" << endl;
  }

  friend istream &operator >> (istream &stream,vehicle& obj)
    {
      cout<<"Vehicle name: ";
      stream>>obj.name;
      try
        {
          cout<<"Number of wheels: ";
          stream>>obj.Nwheels;
          if (obj.Nwheels < 2){
              throw "Not correct number of wheels! Try again";
        }
        }
        catch (const char* msg) {
          cerr << "Exception caught: " << msg <<endl;
          obj.Nwheels=InputProve(obj.Nwheels);
        }
        for (int i = 0; i < obj.Nwheels; i++) {
          wheel newWheel;
          obj.vec_wheels.push_back(newWheel);
          }
        try
          {
            cout<<"Tank capacity: ";
            stream>>obj.tank_Capacity;
            if (obj.tank_Capacity<=0){
              throw "Not correct tank capacity! Try again";
            }
          }
          catch (const char* msg)
          {
            cerr << "Exception caught: " << msg <<endl;
            obj.tank_Capacity=InputProve(obj.tank_Capacity);
          }
        try
          {
            cout<<"Engine power: ";
            stream>>obj.engine_Power;
            if (obj.engine_Power<=0){
              throw "Not correct power of engine! Try again";
            }
          }
          catch (const char* msg)
          {
            cerr << "Exception caught: " << msg <<endl;
            obj.engine_Power=InputProve(obj.engine_Power);
          }
          obj.fuel_Level=obj.tank_Capacity;
          obj.def_engine(obj.engine_Power);
          obj.calculatevelocity();
      return stream;
    }

vehicle& operator=(const vehicle& other) {
  name = other.name;
  Nwheels = other.Nwheels;
  velocity = other.velocity;
  mileage = other.mileage;
  current_Distance=other.current_Distance;
  current_Circles=other.current_Circles;
  travel_Time=other.travel_Time;
  return *this;
}
};

int allfinished(vector<vehicle> &v,double trackLen,int circles);
int skip(vector<int> skip_id, int i);

vector<vehicle>RatingResults(vector<vehicle> v);
void outputResults(vector<vehicle> v);



int main() {
  srand(time(NULL));
  vector<vehicle> race_rez;
  vector<vehicle> cars;
  int NumCircles;
  double trackLen = 0;
  int flag = 1;
  int rez = 0;
  while (flag == 1) {
    int choice = 10;
    try
      {
      choice = menu(rez);
      if ((choice > 5) || (choice < 0)) {
        throw "Error, try another number!";
        }
      }
    catch (const char* msg)
      {
        cerr << "Exception caught: " << msg << endl;
        cout<<"Try again ";
        choice = InputProve(choice);
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
      break;
    case (1): {
      clean();
      vehicle addcar;
      cin>>addcar;
      cars.push_back(addcar);
      clean();
      rez = 0;
      break;
    }
    case (2): {
      clean();
      for (int i = 0; i < cars.size(); i++) {
        cout << cars[i];
      }
      break;
    }
    case (3): {
      clean();
      try{
        cout<<"The number of circles: ";
        NumCircles=InputProve(NumCircles);
        if (NumCircles<=0){
          throw "Not correct number of circles! try again";
        }
      }
      catch (const char* msg)
        {
          cerr << "Exception caught: " << msg << endl;
          cout<<"Try again ";
          NumCircles=InputProve(NumCircles);
        }
      cout << "Enter the length of the track (km): ";
      trackLen = InputProve(trackLen);
      rez = 0;
      break;
    }
    case (4): {
      clean();
      if (trackLen == 0 || NumCircles==0) {
        cout << "You didn't enter the length of the track!\n";
        rez = 0;
        break;
      }
      else
      {
        for(int i=0;i<cars.size();i++)
          {
            cars[i].reset();
          }
        double current_travel_Time=0;
        vector<int> skip_id{-1};
        int racing_cars=cars.size();
        while(!allfinished(cars,trackLen,NumCircles))
          {
            if(racing_cars<=0)
              {
                break;
              }
            for (int i = 0; i <cars.size(); i++)
              {
                int exit=0;
                if (find(skip_id.begin(), skip_id.end(), i) == skip_id.end())
                {
                  cars[i].mileage_plus();
                  cars[i].current_Distance_plus();
                  for (int j = 0; j < cars[i].vec_wheels.size(); j++)
                  {
                    cars[i].vec_wheels[j].def_wheel(cars[i].get_mileage(),cars[i].get_velocity());
                  }
                  cars[i].number_of_damaged_wheels();
                  cars[i].calculate_cur_fuel(cars[i].calculateConsumption(),cars[i].get_current_Distance());
                  cars[i].calculatevelocity();
                  cars[i].set_travel_Time(current_travel_Time);
                  cars[i].travel_Time_display();
                  cout<<"Car: "<<cars[i].name
                    <<" Circle: "<<cars[i].get_current_Circles()+1
                    <<" Velocity: "<<cars[i].get_velocity()<<"\n"
                    <<" Current fuel "<<cars[i].get_fuel_Level()
                    <<" Damaged wheels: "<<cars[i].get_damaged_wheels()<<"\n"
                    <<" Mileage: "<<cars[i].get_mileage()<<"\n";
                  if (cars[i].get_damaged_wheels()==cars[i].vec_wheels.size() || (cars[i].get_fuel_Level()<=0))
                  {
                      exit=1;
                      cars[i].set_travel_Time(current_travel_Time);
                      cars[i].current_Circles_plus();
                  }
                  if ((trackLen*NumCircles)-(cars[i].get_mileage())<=0)
                  {
                    exit=1;
                    cars[i].set_travel_Time(current_travel_Time);
                    cars[i].set_current_Circles(NumCircles);
                    cars[i].travel_Time_display();
                    cout<<""<<cars[i].name<<" Just finished"<<endl;
                  }
                  if (exit==1)
                    {
                      skip_id.push_back(i);
                      racing_cars=racing_cars-1;
                      break;
                    }
                  if (cars[i].calc_circles(trackLen))
                    {
                      cars[i].need_refuel(trackLen);
                      cars[i].need_change();
                    }
                }
                else
                {
                  continue;
                }
              }
            current_travel_Time+=dt;
          }
        for(int i=0;i<cars.size();i++)
          {
            cars[i].total_travel_Time();
          }
      }
      break;
    }
    case (5): {
      clean();
      outputResults(cars);
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
  cout << "\nInter a number to choose action:\n";
  cout << "1 Add new vehicle\n";
  cout << "2 Output information about vehicle\n";
  cout << "3 Enter the track\n";
  cout << "4 Start race\n";
  cout << "0 Exit\n";
  if (rez == 1)
    cout << "5 Results of the last race\n";
  int choice = 0;
  choice = InputProve(choice);
  if (choice == 4) {
    rez = 1;
  }
  return choice;
}

vector<vehicle>  RatingResults(vector<vehicle> v) {
    sort(v.begin(), v.end(), []( vehicle& a,  vehicle& b) {
    if (a.get_travel_Time() != b.get_travel_Time()) {
      return a.get_travel_Time() < b.get_travel_Time();
    }
    else {
      return a.get_number_of_refuels() < b.get_number_of_refuels();
    }
  });
  return v;
}

void outputResults(vector<vehicle> v) {
  vector<vehicle> results = RatingResults(v);
  for (int i = 0; i < results.size(); i++) {
    cout << results[i].name << endl;
    results[i].travel_Time_display();
    cout<<"Circles "<<results[i].get_current_Circles()<<endl;
    cout << "Refuel travel_Times: " << int(results[i].get_number_of_refuels()) << endl;
  }
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

void wheel :: def_wheel(double mileage, double velocity) {
  if (mileage==0.) {
    current_Distance=0.;
    status=0.;
  }
  else{
    current_Distance = mileage;
    status = check_status(current_Distance,velocity);
  }
}

int wheel::check_status(double mileage, double velocity) {
  if (status == 0)
  {
  int end = ceil(sqrt(mileage))+ceil(sqrt(velocity));
  int start=-1000;
  int ratio = ceil(((mileage)*(sqrt(velocity)+1)));
  int damageProb = rand() % (end - (start)+ratio)+start;
  if (damageProb - end>=0)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else return 1;
}
void wheel ::output() {
  if (status == 1) {
    cout << "damaged" << endl;
  } else {
    cout << "not damaged" << endl;
  }
}

void engine::def_engine(double power) {
  engine_Power = power;
  engine_Consumption = calculateConsumption();
}

void fuel_system ::calculate_cur_fuel(double engine_Consumption, double mileage) {
    fuel_Level=double(tank_Capacity - ((engine_Consumption / 100) * mileage));
}

void fuel_system ::def_fuel_system() {
  cout << "fuel capacity: ";
  double capacity=0.;
  capacity = InputProve(capacity);
  tank_Capacity = capacity;
  fuel_Level = capacity;
}


void vehicle ::travel_Time_display()
{
  double t = this->travel_Time;
  int hours = int(t);
  double cur_travel_Time = (t - hours) * 60;
  int minutes = int(cur_travel_Time);
  int seconds = int((cur_travel_Time - minutes) * 60);
  cout << "travel_Time: " << setw(4)<<setfill('0')<<hours << ":" << setw(2)<<setfill('0')<<minutes << ":" << setw(2)<<setfill('0')<<seconds << endl;
}

void vehicle ::calculatevelocity()
{
  if (damaged_wheels == 0) {
velocity= double(fabs(sqrt(engine_Power) * (70.0 / double(Nwheels) - 2.5) /sqrt(fuel_Level)));
 }
 else
 {
   velocity = (fabs(sqrt(engine_Power) * (70.0 / double(Nwheels) - 2.5) / sqrt(fuel_Level))*(pow(0.75, double(damaged_wheels))));
     }
}

void vehicle ::number_of_damaged_wheels()
{
  int count = 0;
  for (int i = 0; i < vec_wheels.size(); i++)
  {
    if (vec_wheels[i].get_status() == 1) {
      count++;
    }
  }
  damaged_wheels = count;
}

void vehicle ::need_refuel(double tracklen)
{
  if (fuel_Level < (engine_Consumption*100) * tracklen)
  {
    this->plus_number_of_refuels();
    pit_stop_travel_Time+=this->refuel_travel_Time();
    fuel_Level=tank_Capacity;
  }
}

void vehicle :: need_change()
{
  if (damaged_wheels!=0)
  {
    pit_stop_travel_Time+=this->wheels_change();
    for(int i=0;i<vec_wheels.size();i++)
      {
        if (vec_wheels[i].get_status()==1)
        {
          vec_wheels[i].def_wheel(0.,0.);
        }
      }
  }
}

void vehicle::reset()
 {
   for(int i=0;i<vec_wheels.size();i++)
     {
       vec_wheels[i].set_status(0);
     }
   damaged_wheels=0;
   current_Distance=mileage=0;
   number_of_refuels=current_Circles=0;
   travel_Time=0;
   fuel_Level=tank_Capacity;
   calculatevelocity();
   for (int j = 0; j < vec_wheels.size(); j++)
     {
       vec_wheels[j].def_wheel(mileage,get_velocity());
     }
   cout<<"\n"<<name<<" reseted"<<endl;
 }

int allfinished(vector<vehicle> &v,double trackLen,int circles)
{
  int count=0;
  for (int i=0;i<v.size();i++)
    {
      if (v[i].get_current_Distance()-trackLen*circles>=0)
      {
        count++;
      }
    }
  if (count==v.size())
  {
    return 1;
  }
  else return 0;
}
