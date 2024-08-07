#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <compare>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"
using namespace std;
#endif /* __PROGTEST__ */

class MyPol {
public:
    MyPol(size_t id_company, size_t id_pack, bool type, APolygon polygon): id_company(id_company), id_pack(id_pack), type(type), polygon(polygon) {}
    size_t id_company;
    size_t id_pack;
    bool type; // true - min, false - cnt
    APolygon polygon;
};

using AMyPol = std::shared_ptr<MyPol>;

class SafeQueue {
public:
    void push(AMyPol pack) {
        unique_lock<mutex> lock(mtx_queue);
        safe_queue.push(pack);
    }
    AMyPol front() {
        unique_lock<mutex> lock(mtx_queue);
        return safe_queue.front();
    }
    void pop() {
        unique_lock<mutex> lock(mtx_queue);
        safe_queue.pop();
    }
    bool empty() {
        unique_lock<mutex> lock(mtx_queue);
        return safe_queue.empty();
    }
    queue<AMyPol> safe_queue;
    mutex mtx_queue;
};

class Pack {
public:
    Pack(size_t id_pack, AProblemPack problem_pack): id_pack(id_pack), problem_pack(problem_pack) {
        commen_problem = problem_pack->m_ProblemsMin.size() + problem_pack->m_ProblemsCnt.size();
        if(problem_pack->m_ProblemsMin.empty()) {
            commen_problem++;
        }
        if(problem_pack->m_ProblemsCnt.empty()){
            commen_problem++;
        }
        resolved = 0;
    }
    bool solved() {
        return (resolved == commen_problem);
    }
    size_t id_pack;
    atomic<size_t> resolved;
    atomic<size_t> commen_problem;
    AProblemPack problem_pack;
};

using APack = std::shared_ptr<Pack>;

class MyCompany{
public:
    MyCompany(size_t id_company, ACompany company): id_company(id_company), company(company) {}

    void createThread(atomic<bool> & worker_finished, SafeQueue & safe_queue, condition_variable & cv_worker) {
        sender = thread(&MyCompany::senderFunc, this, ref(safe_queue), ref(cv_worker));
        deliver = thread(&MyCompany::deliverFunc, this, ref(worker_finished));
    }
    void senderFunc(SafeQueue & safe_queue, condition_variable & cv_worker) {
        size_t id_pack = 0;
        while(true) {
            AProblemPack new_pack = company->waitForPack();
            if(!new_pack.get()) {
                break;
            }
            APack my_pack = make_shared<Pack>(id_pack, new_pack);
            unique_lock<mutex> lock(mtx_deliver);
            deliver_queue.push(my_pack);
            lock.unlock();
            unique_lock<mutex> lock_vector(mtx_vector);
            pack_vector.push_back(my_pack);
            lock_vector.unlock();
            for(APolygon polygon_iter: new_pack->m_ProblemsMin) {
                safe_queue.push(make_shared<MyPol>(id_company, id_pack, true, polygon_iter));
            }
            for(APolygon polygon_iter: new_pack->m_ProblemsCnt) {
                safe_queue.push(make_shared<MyPol>(id_company, id_pack, false, polygon_iter));
            }
            if(new_pack->m_ProblemsMin.empty()) {
                safe_queue.push(make_shared<MyPol>(id_company, id_pack, true, nullptr));
            }
            if(new_pack->m_ProblemsCnt.empty()) {
                safe_queue.push(make_shared<MyPol>(id_company, id_pack, false, nullptr));
            }
            id_pack++;
            cv_worker.notify_one();
        }
    }

    void deliverFunc(atomic<bool> & workers_finished) {
        while(true) {
            unique_lock<mutex> lock(mtx_deliver);
            cv_deliver.wait(lock, [&](){ return (!deliver_queue.empty() && deliver_queue.front()->solved()) || (workers_finished) ;} );
            if(deliver_queue.empty() && workers_finished) {
                break;
            }
            auto solved = deliver_queue.front();
            deliver_queue.pop();
            company->solvedPack(solved->problem_pack);
        }
    }
    void company_notify(AMyPol my_pol) {
        unique_lock<mutex> lock_vector(mtx_vector);
        pack_vector[my_pol->id_pack]->resolved++;
        cv_deliver.notify_one();
    }
    size_t id_company;
    ACompany company;
    thread sender;
    thread deliver;
    queue<APack> deliver_queue;
    mutex mtx_deliver;
    condition_variable cv_deliver;
    vector<APack> pack_vector;
    mutex mtx_vector;
};

using AMYCompany = std::shared_ptr<MyCompany>;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
class COptimizer
{
  public:
    static bool                        usingProgtestSolver                     ( void )
    {
      return true;
    }
    static void                        checkAlgorithmMin                       ( APolygon                              p )
    {
      // dummy implementation if usingProgtestSolver() returns true
    }
    static void                        checkAlgorithmCnt                       ( APolygon                              p )
    {
      // dummy implementation if usingProgtestSolver() returns true
    }
    void                               start                                   ( int                                   threadCount ) {
        senders_finished = false;
        workers_finished = false;
        solver_cnt = createProgtestCntSolver();
        solver_min = createProgtestMinSolver();
        for(int i = 0; i < threadCount; i++) {
            workers.emplace_back(&COptimizer::workersFunc, this);
        }
        for(shared_ptr<MyCompany> company: companies) {
            company->createThread(workers_finished, safe_queue, cv_workers);
        }
    }
    void                               stop                                    ( void ) {
        for(AMYCompany company: companies) {
            company->sender.join();
        }
        senders_finished = true;
        cv_workers.notify_all();
        for(thread & worker: workers) {
            worker.join();
        }
        workers_finished = true;
        for(AMYCompany company: companies) {
            company->cv_deliver.notify_all();
            company->deliver.join();
        }
    }
    void                               addCompany                              ( ACompany                              company ) {
        size_t id_company = companies.size();
        companies.push_back(make_shared<MyCompany>(id_company, company));
    }
    void workersFunc() {
        while(true) {
            unique_lock<mutex> lock(mtx_workers);
            cv_workers.wait(lock, [this](){return (!safe_queue.empty()) || senders_finished; });
            if(safe_queue.empty() && senders_finished && vector_cnt.empty() && vector_min.empty()) {
                lock.unlock();
                cv_workers.notify_one();
                break;
            } else if(safe_queue.empty() && senders_finished && !vector_cnt.empty()) {
                vector<AMyPol> current_pols = vector_cnt;
                vector_cnt.clear();
                lock.unlock();
                cv_workers.notify_one();
                solveProblems(solver_cnt,current_pols);
            } else if(safe_queue.empty() && senders_finished && !vector_min.empty()) {
                vector<AMyPol> current_pols = vector_min;
                vector_min.clear();
                lock.unlock();
                cv_workers.notify_one();
                solveProblems(solver_min, current_pols);
            } else {
                while(!safe_queue.empty()) {
                    AMyPol my_pol = safe_queue.front();
                    safe_queue.pop();
                    if(my_pol->type) {
                        if(my_pol->polygon.get()) {
                            solver_min->addPolygon(my_pol->polygon);
                        }
                        vector_min.push_back(my_pol);
                        if(!solver_min->hasFreeCapacity()) {
                            AProgtestSolver current_solver = solver_min;
                            vector<AMyPol> current_pols = vector_min;
                            solver_min = createProgtestMinSolver();
                            vector_min.clear();
                            lock.unlock();
                            cv_workers.notify_one();
                            solveProblems(current_solver, current_pols);
                            break;
                        }
                    } else {
                        if(my_pol->polygon.get()) {
                            solver_cnt->addPolygon(my_pol->polygon);
                        }
                        vector_cnt.push_back(my_pol);
                        if(!solver_cnt->hasFreeCapacity()) {
                            AProgtestSolver current_solver = solver_cnt;
                            vector<AMyPol> current_pols = vector_cnt;
                            solver_cnt = createProgtestCntSolver();
                            vector_cnt.clear();
                            lock.unlock();
                            cv_workers.notify_one();
                            solveProblems(current_solver, current_pols);
                            break;
                        }
                    }
                }
            }
        }
    }
    void solveProblems(AProgtestSolver solver, vector<AMyPol> & vec_polygons) {
        solver->solve();
        for(AMyPol one_polygon: vec_polygons) {
            companies[one_polygon->id_company]->company_notify(one_polygon);
        }
    }
  private:
    AProgtestSolver solver_min;
    vector<AMyPol> vector_min;
    AProgtestSolver solver_cnt;
    vector<AMyPol> vector_cnt;
    vector<thread> workers;
    atomic<bool> senders_finished;
    atomic<bool> workers_finished;
    mutex mtx_workers;
    condition_variable cv_workers;
    vector<AMYCompany> companies;
    SafeQueue safe_queue;
};
// TODO: COptimizer implementation goes here
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
int                                    main                                    ( void )
{
  COptimizer optimizer;
  ACompanyTest  company = std::make_shared<CCompanyTest> ();
  optimizer . addCompany ( company );
  optimizer . start ( 4 );
  optimizer . stop  ();
  if ( ! company -> allProcessed () )
    throw std::logic_error ( "(some) problems were not correctly processsed" );
  return 0;
}
#endif /* __PROGTEST__ */
