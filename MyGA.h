//
// Created by googl on 24-10-24.
//三极管

#ifndef MYGA_H
#define MYGA_H
#include <functional>
#include <cmath>
#include <random>
// 遗传算法求一元函数最值问题
template<typename T=double>
class MyGA {
private:
    std::function<T(T)> m_opt_func;
    size_t m_N_points{};
    size_t m_cho_num{};
    const double m_p_cross;
    const double m_p_mutate;
    T m_x_low;
    T m_x_up;
    class Chromo: public std::vector<bool> {
    public:
        explicit Chromo(const size_t chr_size) {
            this->resize(chr_size);
        }
    };
public:
    struct ga_result {
        T input;
        T value;
    };
    explicit MyGA(std::function<T(T)> opt_func, const double p_cross=0.7, const double p_mutate=0.001) : m_opt_func(opt_func), m_p_cross(p_cross), m_p_mutate(p_mutate) {}
    ga_result solve(T x_low, T x_up, double acc_min, size_t cho_num, size_t gen_num) {
        m_N_points=std::ceil(log2((x_up-x_low)/acc_min));
        m_cho_num=cho_num;
        m_x_low=x_low;
        m_x_up=x_up;
        init_chromosomes(cho_num, m_N_points);
        T temp_x, temp_num;
        auto best=getBest(temp_x, temp_num);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> ur(0.0, 1.0);
        std::uniform_int_distribution<> ud(0, m_N_points-1);
        for(size_t i=0;i<gen_num;i++) {
            select();
            long long pre=-1;
            for(size_t j=0;j<m_cho_num;j++) {
                if(ur(gen)<m_p_cross) {
                    if(pre==-1) {
                        pre=j;
                    }else {
                        cross(m_chromosomes[pre],m_chromosomes[j], ud(gen));
                        pre=-1;
                    }
                }
            }
            for(size_t k=0;k<m_cho_num;k++) {
                if(ur(gen)<m_p_mutate) {
                    mutate(m_chromosomes[k]);
                }
            }
        }
        T input,val;
        getBest(input,val);
        return {input,val};
    }
private:
    std::vector<Chromo> m_chromosomes;
    void init_chromosomes(size_t cho_num, size_t len_cho) {
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::uniform_int_distribution<> dis(0,1);
        for(size_t i=0;i<cho_num;i++) {
            Chromo temp_cho(len_cho);
            for(size_t j=0;j<len_cho;j++) {
                temp_cho[j]=dis(gen);
            }
            m_chromosomes.push_back(temp_cho);
        }
    }
    T decode(Chromo &ch, T x_low, T x_up) {
        double ratio{0};
        size_t i=0;
        for(auto value:ch) {
            if(value) {
                ratio+=1.0/pow(2, m_N_points-i);
            }
            i++;
        }
        return x_low+(x_up-x_low)*ratio;
    }
    T fitness(Chromo &ch, T x_low, T x_up) {
        return m_opt_func(decode(ch, x_low, x_up));
    }
    void select() {
        std::vector<T> fit_values, avg_fit_values, temp_ids;
        temp_ids.resize(m_cho_num);
        fit_values.resize(m_cho_num);
        avg_fit_values.resize(m_cho_num);
        T sum=0;
        for(size_t i=0;i<m_cho_num;i++) {
            fit_values[i]=fitness(m_chromosomes[i], m_x_low, m_x_up);
            sum+=fit_values[i];
        }
        for(size_t i=0;i<m_cho_num;i++) {
            avg_fit_values[i]=fit_values[i]/sum;
        }
        for(size_t i=1;i<m_cho_num;i++) {
            avg_fit_values[i]+=avg_fit_values[i-1];
        }
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::uniform_real_distribution<>  dis(0,1);
        for(size_t i=0;i<m_cho_num;i++) {
            auto rnd=dis(gen);
            size_t j=0;
            for(;j<m_cho_num-1;j++) {
                if(rnd<avg_fit_values[j]) {
                    temp_ids[i]=j;
                    break;
                }
            }
            if(j==m_cho_num-1) {
                temp_ids[i]=j;
            }
        }
        auto copy_cho=m_chromosomes;
        for(size_t i=0;i<m_cho_num;i++) {
            m_chromosomes[i]=copy_cho[temp_ids[i]];
        }
    }
    void mutate(Chromo &ch) {
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::uniform_int_distribution<size_t> dis(0, m_N_points-1);
        ch[dis(gen)]=!ch[dis(gen)];
    }
    void cross(Chromo &ch1, Chromo &ch2, size_t point) {
        auto copy_ch1=ch1;
        for(int i=0;i<m_N_points-point;i++) {
            ch1[point+i]=ch2[point+i];
        }
        for(int i=0;i<m_N_points-point;i++) {
            ch2[point+i]=copy_ch1[point+i];
        }
    }
    size_t getBest(T &_input, T &_num) {
        std::vector<T> fitnessVal;
        fitnessVal.resize(m_cho_num);
        for(size_t i=0;i<m_cho_num;i++) {
            fitnessVal[i]=fitness(m_chromosomes[i], m_x_low, m_x_up);
        }
        size_t id=0;
        for(size_t i=1;i<m_cho_num;i++) {
            if(fitnessVal[i]>fitnessVal[id]) {
                id=i;
            }
        }
        _input=decode(m_chromosomes[id], m_x_low, m_x_up);
        _num=m_opt_func(_input);
        return id;
    }
};



#endif //MYGA_H
