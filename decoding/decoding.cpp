#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;
using Symbol = char;
using Word = std::vector<Symbol>;

struct Grammar {
    std::set<Symbol> m_Nonterminals;
    std::set<Symbol> m_Terminals;
    std::vector<std::pair<Symbol, std::vector<Symbol>>> m_Rules;
    Symbol m_InitialSymbol;
};
#endif

std::vector<size_t> trace(const Grammar& g, const Word& x) {
    for(size_t i = 0; i < g.m_Rules.size(); i++){
        if(g.m_Rules[i].first == g.m_InitialSymbol && g.m_Rules[i].second.empty() && x.empty()){
            return vector<size_t>{i};
        }
    }
    size_t size = x.size();

    vector<vector<map<Symbol, vector<size_t>>>> P(size, vector<map<Symbol, vector<size_t>>>(size));
    map<Symbol, vector<size_t>> result;

    for (size_t word_index = 0; word_index < size; ++word_index) {
        for(size_t rule_index = 0; rule_index < g.m_Rules.size(); ++rule_index) {
            if(g.m_Rules[rule_index].second.size() == 1 && g.m_Rules[rule_index].second[0] == x[word_index]) {
                P[word_index][0][g.m_Rules[rule_index].first].push_back(rule_index);
                if(x.size() == 1) result[g.m_Rules[rule_index].first] = (vector<size_t>{rule_index});
            }
        }
    }

    // CYK algorithm
    for (size_t column_index = 1; column_index < size; ++column_index) {
        for (size_t row_index = 0; row_index < size - column_index; ++row_index) {
            for (size_t k = 0; k < column_index; ++k) {
                auto& left_hand = P[row_index][k];
                auto& right_hand = P[row_index + k + 1][column_index - k - 1];
                if(left_hand.empty() || right_hand.empty()) continue;
                for(auto & left_pair: left_hand) {
                    for(auto & right_pair: right_hand) {
                        vector<Symbol> combination = {left_pair.first, right_pair.first};
                        for(size_t rule_index = 0; rule_index < g.m_Rules.size(); ++rule_index) {
                            if(g.m_Rules[rule_index].second == combination) {
                                vector<size_t> rule_indices;
                                rule_indices.push_back(rule_index);
                                rule_indices.insert(rule_indices.end(), left_pair.second.begin(), left_pair.second.end());
                                rule_indices.insert(rule_indices.end(), right_pair.second.begin(), right_pair.second.end());
                                P[row_index][column_index][g.m_Rules[rule_index].first] = rule_indices;
                                if(g.m_Rules[rule_index].first == g.m_InitialSymbol && row_index == 0 && column_index == size - 1) {
                                    result[g.m_InitialSymbol] = rule_indices;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return result[g.m_InitialSymbol];
}


#ifndef __PROGTEST__
int main()
{
    Grammar g0{
            {'A', 'B', 'C', 'S'},
            {'a', 'b'},
            {
                    {'S', {'A', 'B'}},
                    {'S', {'B', 'C'}},
                    {'A', {'B', 'A'}},
                    {'A', {'a'}},
                    {'B', {'C', 'C'}},
                    {'B', {'b'}},
                    {'C', {'A', 'B'}},
                    {'C', {'a'}},
            },
            'S'};

    assert(trace(g0, {'b', 'a', 'a', 'b', 'a'}) == std::vector<size_t>({0, 2, 5, 3, 4, 6, 3, 5, 7}));
    assert(trace(g0, {'b'}) == std::vector<size_t>({}));
    assert(trace(g0, {'a'}) == std::vector<size_t>({}));
    assert(trace(g0, {}) == std::vector<size_t>({}));
    assert(trace(g0, {'a', 'a', 'a', 'a', 'a'}) == std::vector<size_t>({1, 4, 6, 3, 4, 7, 7, 7, 7}));
    assert(trace(g0, {'a', 'b'}) == std::vector<size_t>({0, 3, 5}));
    assert(trace(g0, {'b', 'a'}) == std::vector<size_t>({1, 5, 7}));
    assert(trace(g0, {'c', 'a'}) == std::vector<size_t>({}));

    Grammar g1{
            {'A', 'B'},
            {'x', 'y'},
            {
                    {'A', {}},
                    {'A', {'x'}},
                    {'B', {'x'}},
                    {'A', {'B', 'B'}},
                    {'B', {'B', 'B'}},
            },
            'A'};

    assert(trace(g1, {}) == std::vector<size_t>({0}));
    assert(trace(g1, {'x'}) == std::vector<size_t>({1}));
    assert(trace(g1, {'x', 'x'}) == std::vector<size_t>({3, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 2, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 2, 2, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2}));
    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2}));

    Grammar g2{
            {'A', 'B'},
            {'x', 'y'},
            {
                    {'A', {'x'}},
                    {'B', {'x'}},
                    {'A', {'B', 'B'}},
                    {'B', {'B', 'B'}},
            },
            'B'};

    assert(trace(g2, {}) == std::vector<size_t>({}));
    assert(trace(g2, {'x'}) == std::vector<size_t>({1}));
    assert(trace(g2, {'x', 'x'}) == std::vector<size_t>({3, 1, 1}));
    assert(trace(g2, {'x', 'x', 'x'}) == std::vector<size_t>({3, 3, 1, 1, 1}));

    Grammar g3{
            {'A', 'B', 'C', 'D', 'E', 'S'},
            {'a', 'b'},
            {
                    {'S', {'A', 'B'}},
                    {'S', {'S', 'S'}},
                    {'S', {'a'}},
                    {'A', {'B', 'S'}},
                    {'A', {'C', 'D'}},
                    {'A', {'b'}},
                    {'B', {'D', 'D'}},
                    {'B', {'b'}},
                    {'C', {'D', 'E'}},
                    {'C', {'b'}},
                    {'C', {'a'}},
                    {'D', {'a'}},
                    {'E', {'S', 'S'}},
            },
            'S'};

    assert(trace(g3, {}) == std::vector<size_t>({}));
    assert(trace(g3, {'b'}) == std::vector<size_t>({}));
    assert(trace(g3, {'a', 'b', 'a', 'a', 'b'}) == std::vector<size_t>({1, 2, 0, 3, 7, 1, 2, 2, 7}));
    assert(trace(g3, {'a', 'b', 'a', 'a', 'b', 'a', 'b', 'a', 'b', 'a', 'a'}) == std::vector<size_t>({1, 1, 0, 4, 8, 11, 12, 0, 5, 6, 11, 11, 0, 4, 9, 11, 7, 11, 7, 2, 2}));
}
#endif

