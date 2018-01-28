#ifndef TIMEMEASUREMENT_H_INCLUDED
#define TIMEMEASUREMENT_H_INCLUDED
//#ifdef __GXX_EXPERIMENTAL_CXX0X__
#include <chrono>

class time_count//mainly for debugging. Time point 1 = cstr, Time point 2 = dstr or operator ().
{
    std::chrono::high_resolution_clock::time_point first_time_point;
    std::chrono::high_resolution_clock::time_point second_time_point;

    public:
//    const void reset(){first_time_point(); second_time_point();}
//    void operator() (const std::string to_do_what)
//    {
//        second_time_point=std::chrono::high_resolution_clock::now();
//        std::cout << "It took " <<
//        std::chrono::duration_cast<std::chrono::duration<double>>
//        (second_time_point - first_time_point).count()
//         << " seconds to "<<to_do_what<<"."<<"\n";
//    }
//    void operator() (const std::string to_do_what, const unsigned cycles)
//    {
//        second_time_point=std::chrono::high_resolution_clock::now();
//        double t=std::chrono::duration_cast<std::chrono::duration<double>>(second_time_point - first_time_point).count();
//        std::cout << "It took " << t << " seconds to "<<to_do_what<<"."<<"\n";
//        std::cout << "It's "<< t/cycles <<" second(s) per loop cycle."<<"\n";
//    }
    void reset() {first_time_point = std::chrono::high_resolution_clock::now();}
    void start() {first_time_point = std::chrono::high_resolution_clock::now();}
    time_count& stop() { second_time_point = std::chrono::high_resolution_clock::now(); return *this; }
    friend std::ostream& operator<< (std::ostream& os, const time_count& tc) {
//        if (tc.second_time_point==0) tc.stop();
        const double duration = std::chrono::duration_cast< std::chrono::duration<double> >
        (tc.second_time_point - tc.first_time_point).count();
        if (duration<60.0) os << "It took " << duration << " seconds ";
        else if (duration<3600.0) os << "It took " << duration / 60.0 << " minutes (numbers after dot are NOT seconds)";
        else if (duration<864000) os << "It took " << duration / 60.0 << " minutes (numbers after dot are NOT minutes)";
        else os << "It took " << duration / 24.0 << " days ";
        return os; }
    time_count() : first_time_point (std::chrono::high_resolution_clock::now()), second_time_point() { }
    virtual ~time_count(){  }
};


//#endif
#endif // TIMEMEASUREMENT_H_INCLUDED
