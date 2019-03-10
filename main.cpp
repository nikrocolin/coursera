#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses
};

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

istream& operator >> (istream& is, Query& q) {
    string tip;
    is >> tip;
    if (tip == "NEW_BUS") {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count;
        is >> stop_count;
        q.stops.resize(stop_count);
        for (string& stop : q.stops) {
          is >> stop;
        }

    } else if (tip == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (tip == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if (tip == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }
  return is;
}

struct BusesForStopResponse {
  vector <string> buses;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
    if (r.buses.size() == 0) {
           os << "No stop";
       } else {
           int i = 1;
           for (auto& q : r.buses) {
               os << q;
               if (i < r.buses.size()) {
                   cout << " ";
               }
               ++i;
           }
       }
    os<<endl;
  return os;
}

struct StopsForBusResponse {
    vector <string> stops;
    map<string, vector<string>> stops_to_buses;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
    if (r.stops_to_buses.size() == 0) {
           os << "No bus";
        } else {
            int i = 1;
            for (auto& stop : r.stops) {
                os << "Stop " << stop << ": ";
                if (r.stops_to_buses.count(stop) == 0) {
                    os << " no interchange";
                } else {
                    for (auto& bus : r.stops_to_buses.at(stop)) {
                        os << " " << bus;
                    }
                }
                if (i < r.stops.size()) {
                    os << endl;
                }
                ++i;
            }
        }
        os << endl;
        return os;
}

struct AllBusesResponse {
   map<string, vector<string>> buses_to_stops;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
    if (r.buses_to_stops.size() == 0) {
            os << "No buses";
        } else {
            int i = 1;
            for (auto& bus : r.buses_to_stops) {
                os << "Bus " << bus.first << ":";
                for (auto& stop : bus.second) {
                    os << " " << stop;
                }
                if (i < r.buses_to_stops.size()) {
                    os << endl;
                }
                ++i;
            }
        }
    os<<endl;
  return os;
}

class BusManager {
public:
  void AddBus(const string& bus, const vector<string>& stops) {
      for (auto& q: stops) {
                stops2buses[q].push_back(bus);
                buses2stops[bus].push_back(q);
            }
  }

  BusesForStopResponse GetBusesForStop(const string& stop) const {
      BusesForStopResponse result;
           if (stops2buses.count(stop) > 0) {
               result.buses = stops2buses.at(stop);
           }
           return result;
  }

  StopsForBusResponse GetStopsForBus(const string& bus) const {
      StopsForBusResponse result;
             if (buses2stops.count(bus) > 0) {
                 for (auto& stop : buses2stops.at(bus)) {
                     result.stops.push_back(stop);
                     for (auto& bus_n : GetBusesForStop(stop).buses) {
                         if (bus_n != bus) {
                             result.stops_to_buses[stop].push_back(bus_n);
                         }
                     }
                 }
             }
             return result;
  }

  AllBusesResponse GetAllBuses() const {
      AllBusesResponse result;
            result.buses_to_stops = buses2stops;
            return result;
  }
private:
  map <string, vector<string>> stops2buses;
  map <string, vector<string>> buses2stops;
};



int main() {
  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus) << endl;
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses() << endl;
      break;
    }
  }

  return 0;
}

