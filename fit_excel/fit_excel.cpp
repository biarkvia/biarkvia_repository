#ifndef __PROGTEST__


class CExprBuilder
{
  public:
    virtual void                       opAdd                                   () = 0;
    virtual void                       opSub                                   () = 0;
    virtual void                       opMul                                   () = 0;
    virtual void                       opDiv                                   () = 0;
    virtual void                       opPow                                   () = 0;
    virtual void                       opNeg                                   () = 0;
    virtual void                       opEq                                    () = 0;
    virtual void                       opNe                                    () = 0;
    virtual void                       opLt                                    () = 0;
    virtual void                       opLe                                    () = 0;
    virtual void                       opGt                                    () = 0;
    virtual void                       opGe                                    () = 0;
    virtual void                       valNumber                               ( double                                val ) = 0;
    virtual void                       valString                               ( std::string                           val ) = 0;
    virtual void                       valReference                            ( std::string                           val ) = 0;
    virtual void                       valRange                                ( std::string                           val ) = 0;
    virtual void                       funcCall                                ( std::string                           fnName,
                                                                                 int                                   paramCount ) = 0;
};

void                                   parseExpression                         ( std::string                           expr,
                                                                                 CExprBuilder                        & builder );

using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;

constexpr unsigned                     SPREADSHEET_CYCLIC_DEPS                 = 0x01;
constexpr unsigned                     SPREADSHEET_FUNCTIONS                   = 0x02;
constexpr unsigned                     SPREADSHEET_FILE_IO                     = 0x04;
constexpr unsigned                     SPREADSHEET_SPEED                       = 0x08;
constexpr unsigned                     SPREADSHEET_PARSER                      = 0x10;
#endif /* __PROGTEST__ */

using namespace std;

class CPos
{
public:
    CPos() = default;
    CPos(std::string_view str) {
        cell_parser_func(str);
    }
    CPos(const CPos & pos): m_pos_str(pos.m_pos_str), m_column_str(pos.m_column_str), m_column(pos.m_column), m_row(pos.m_row) {}
    CPos(int column, int row) {
        if(column < 0 || row < 0) {
            throw std::invalid_argument("Error CPos copy constructor");
        }
        m_column = column;
        m_row = row;
        std::string result;
        int m_column_index = m_column;
        while(m_column_index >= 0) {
            char symbol = 'A' + (m_column_index % 26);
            result += symbol;
            m_column_index = (m_column_index / 26) - 1;
        }
        std::reverse(result.begin(), result.end());
        m_column_str = result;
        m_pos_str = m_column_str + to_string(m_row);
    }
    CPos & operator=(const CPos & pos) {
        if(this != & pos) {
            m_column = pos.m_column;
            m_row = pos.m_row;
            m_column_str = pos.m_column_str;
            m_pos_str = pos.m_pos_str;
        }
        return * this;
    }
    void cell_parser_func(std::string_view str) {
        string column;
        int column_int = 0;
        size_t index = 0;
        while (index < str.size() && str[index] == ' ') {
            throw std::invalid_argument("Error");
        }
        index = 0;
        while(index < str.size() && std::isalpha(str[index])) {
            char symbol = std::toupper(str[index]);
            if(symbol < 'A' || symbol > 'Z') {
                throw std::invalid_argument("Error");
            }
            column_int = column_int * 26 + (symbol - 'A' + 1);
            column += symbol;
            index++;
        }
        column_int--;
        if(column_int < 0) {
            throw std::invalid_argument("Error");
        }
        if(column.empty() || index == str.size()) {
            throw std::invalid_argument("Error");
        }
        m_column = column_int;
        m_column_str = column;
        std::string row_part = std::string(str.substr(index));
        for(char symbol: row_part) {
            if(!std::isdigit(symbol)) {
                throw std::invalid_argument("Error");
            }
        }
        if(std::stoi(row_part) < 0) {
            throw std::invalid_argument("Error");
        } else {
            m_row = std::stoi(row_part);
        }
        m_pos_str = m_column_str + to_string(m_row);
    }
    bool operator==(const CPos & b) const {
        return m_column == b.m_column && m_row == b.m_row;
    }
    bool operator!=(const CPos & b) const {
        return !(* this == b);
    }
    bool operator<(const CPos & b) const {
        if(m_column == b.m_column) {
            return m_row < b.m_row;
        }
        return m_column < b.m_column;
    }
    int get_column() const {
        return m_column;
    }
    int get_row() const {
        return m_row;
    }
    string get_column_str() const {
        return m_column_str;
    }
    string get_pos_str() const {
        return m_pos_str;
    }
private:
    string m_pos_str;
    string m_column_str;
    int m_column;
    int m_row;
};

pair<int, int> cell_parser_func(std::string_view str) {
    int m_column;
    int m_row;
    string column;
    int column_int = 0;
    size_t index = 0;
    while(index < str.size() && std::isalpha(str[index])) {
        char symbol = std::toupper(str[index]);
        if(symbol < 'A' || symbol > 'Z') {
            throw std::invalid_argument("Error cell_parser_func");
        }
        column_int = column_int * 26 + (symbol - 'A' + 1);
        column += symbol;
        index++;
    }
    column_int--;
    if(column_int < 0) {
        throw std::invalid_argument("Error cell_parser_func");
    }
    if(column.empty() || index == str.size()) {
        throw std::invalid_argument("Error cell_parser_func");
    }
    m_column = column_int;
    std::string row_part = std::string(str.substr(index));
    for(char symbol: row_part) {
        if(!std::isdigit(symbol)) {
            throw std::invalid_argument("Error cell_parser_func");
        }
    }
    if(std::stoi(row_part) < 0) {
        throw std::invalid_argument("Error cell_parser_func");
    } else {
        m_row = std::stoi(row_part);
    }
    return {m_column, m_row};
}

bool cell_parser_func_check(std::string_view str) {
    string column;
    int column_int = 0;
    size_t index = 0;
    while(index < str.size() && std::isalpha(str[index])) {
        char symbol = std::toupper(str[index]);
        if(symbol < 'A' || symbol > 'Z') {
            return false;
        }
        column_int = column_int * 26 + (symbol - 'A' + 1);
        column += symbol;
        index++;
    }
    column_int--;
    if(column_int < 0) {
        return false;
    }
    if(column.empty() || index == str.size()) {
        return false;
    }
    std::string row_part = std::string(str.substr(index));
    for(char symbol: row_part) {
        if(!std::isdigit(symbol)) {
            return false;
        }
    }
    if(std::stoi(row_part) < 0) {
        return false;
    }
    return true;
}

// --------------------------------------------------------------------------------------------------------------------- //

class MyExprBuilder;

class Element {
protected:
    std::monostate value;
public:
    virtual ~Element() {}
    virtual void apply(MyExprBuilder & builder) const = 0;
};
class EmptyElement: public Element {
public:
    virtual void apply(MyExprBuilder & builder) const override {
    }
};
class BinaryOperation: public Element {
protected:
    string operation;
public:
    BinaryOperation(const string & op) : operation(op) {}
    virtual void apply(MyExprBuilder & builder) const override;
    string get_element_value() const {
        return operation;
    }
};
class UnaryOperation: public Element {
protected:
    string operation;
public:
    UnaryOperation(const string & op) : operation(op) {}
    virtual void apply(MyExprBuilder & builder) const override;
    string get_element_value() const {
        return operation;
    }
};
class Number: public Element {
    double value;
public:
    Number(double val): value(val) {}
    virtual void apply(MyExprBuilder & builder) const override;
    double get_element_value() const {
        return value;
    }
};
class String: public Element {
    std::string value;
public:
    String(const string & val): value(val) {}
    virtual void apply(MyExprBuilder & builder) const override;
    string get_element_value() const {
        return value;
    }
};
class Reference: public Element {
    string value;
public:
    Reference(const std::string & val) : value(val) {}
    virtual void apply(MyExprBuilder & builder) const override;
    string get_element_value() const {
        return value;
    }
};

class MyCell {
public:
    virtual ~MyCell() {}
    virtual std::shared_ptr<MyCell> clone() const = 0;
    virtual CValue get_cell_value() const = 0;
    std::deque<std::shared_ptr<Element>> get_deque() const {
        return m_deque;
    }
    void set_deque(const std::deque<std::shared_ptr<Element>> & deque) {
        m_deque = deque;
    }
    deque<std::shared_ptr<Element>> m_deque;
};

class MyNumber: public MyCell {
public:
    MyNumber(const CPos & pos, double value, const std::deque<std::shared_ptr<Element>> & deque): m_value(value) {
        set_deque(deque);
    }
    MyNumber(const MyNumber & from, const std::deque<std::shared_ptr<Element>> & deque): m_value(from.m_value) {
        set_deque(deque);
    }
    MyNumber& operator=(const MyNumber & from) {
        if(this != & from) {
            m_value = from.m_value;
            set_deque(m_deque);
        }
        return * this;
    }
    std::shared_ptr<MyCell> clone() const override {
        return std::make_shared<MyNumber>(* this);
    }
    CValue get_cell_value() const {
        return m_value;
    }
    double get_cell_value_double() const {
        return m_value;
    }
private:
    double m_value = 0;
};

class MyText: public MyCell {
public:
    MyText(const CPos & pos, const std::string & value, const std::deque<std::shared_ptr<Element>> & deque): m_value(value) {
        set_deque(deque);
    }
    MyText(const MyText & from, const std::deque<std::shared_ptr<Element>> & deque): m_value(from.m_value) {
        set_deque(deque);
    }
    MyText & operator=(const MyText & from) {
        if(this != & from) {
            set_deque(m_deque);
            m_value = from.m_value;
        }
        return * this;
    }
    std::shared_ptr<MyCell> clone() const override {
        return std::make_shared<MyText>(* this);
    }
    CValue get_cell_value() const {
        return m_value;
    }
    string get_cell_value_str() const {
        return m_value;
    }
private:
    string m_value = "";
};

class MyEmpty: public MyCell {
public:
    MyEmpty(const CPos & pos, const std::monostate & value, const std::deque<std::shared_ptr<Element>> & deque): m_value(value) {
        set_deque(deque);
    }
    MyEmpty(const MyEmpty & from, const std::deque<std::shared_ptr<Element>> & deque): m_value(from.m_value) {
        set_deque(deque);
    }
    MyEmpty & operator=(const MyEmpty & from) {
        if(this != & from) {
            set_deque(m_deque);
            m_value = from.m_value;
        }
        return * this;
    }
    std::shared_ptr<MyCell> clone() const override {
        return std::make_shared<MyEmpty>(* this);
    }
    CValue get_cell_value() const {
        return m_value;
    }
private:
    std::monostate m_value;
};

class CSpreadsheet
{
public:
    static unsigned                    capabilities                            ()
    {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FILE_IO;
    }
    CSpreadsheet                            () = default;
    bool                               load                                    ( std::istream                        & is );
    bool                               save                                    ( std::ostream                        & os ) const;
    bool                               setCell                                 ( CPos                                  pos,
                                                                                 std::string                           contents );
    CValue                             getValue                                ( CPos                                  pos );
    std::shared_ptr<MyCell> getCell(const CPos & pos);
    void                               copyRect                                ( CPos                                  dst,
                                                                                 CPos                                  src,
                                                                                 int                                   w = 1,
                                                                                 int                                   h = 1 );
    bool dfs_check(const std::string & node);
    bool is_cyclic(const std::string & node);
    friend class MyExprBuilder;
private:
    map<CPos, shared_ptr<MyCell>> m_spreedsheet;
    unordered_map<string, vector<string>> m_dependences_list;
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, bool> current;
};

class MyExprBuilder: public CExprBuilder {
public:
    MyExprBuilder() = default;
    MyExprBuilder(CSpreadsheet * spreadsheet, const std::string & pos, deque<shared_ptr<Element>> & deque):
            m_spreadsheet(spreadsheet), ref_pos_str(pos), m_deque(deque) {
        m_is_deque = true;
    }
    MyExprBuilder(CSpreadsheet * spreadsheet, const std::string & pos, deque<shared_ptr<Element>> * ref_deque):
            m_spreadsheet(spreadsheet), ref_pos_str(pos), m_ref_deque(ref_deque) {
        m_is_ref_deque = true;
    }
    MyExprBuilder(CSpreadsheet * spreadsheet, const std::string & pos, deque<shared_ptr<Element>> & deque, std::deque<shared_ptr<Element>> * ref_deque, int offset_column, int offset_row):
            m_spreadsheet(spreadsheet), ref_pos_str(pos), m_deque(deque), m_ref_deque(ref_deque), m_offset_column(offset_column), m_offset_row(offset_row) {
        m_is_ref_deque = true;
        m_is_deque = true;
    }

    void opAdd() override {
        cout << "+" << endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>("+"));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            String * str1 = dynamic_cast<String *>(op1.get());
            String * str2 = dynamic_cast<String *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                m_deque.push_back(make_shared<Number>(value1 + value2));
            } else if(str1 && str2) {
                string value1 = str1->get_element_value();
                string value2 = str2->get_element_value();
                m_deque.push_back(make_shared<String>(value1 + value2));
            } else if(str1 && num2) {
                string value1 = str1->get_element_value();
                double value2 = num2->get_element_value();
                m_deque.push_back(make_shared<String>(value1 + std::to_string(value2)));
            } else if(num1 && str2) {
                double value1 = num1->get_element_value();
                string value2 = str2->get_element_value();
                m_deque.push_back(make_shared<String>(to_string(value1) + value2));
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void opSub() override {
        std::cout << "-" << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>("-"));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                m_deque.push_back(make_shared<Number>(value1 - value2));
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void opMul() override {
        std::cout << "*" << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>("*"));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                m_deque.push_back(make_shared<Number>(value1 * value2));
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void opDiv() override {
        std::cout << "/" << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>("/"));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                if(value2 == 0) {
                    return;
                } else {
                    m_deque.push_back(make_shared<Number>(value1 / value2));
                }
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void opPow() override {
        std::cout << "^" << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>("^"));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                m_deque.push_back(make_shared<Number>(std::pow(value1, value2)));
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void opNeg() override {
        std::cout << "Negation: " << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<UnaryOperation>("!"));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 1) {
                return;
            }
            shared_ptr<Element> op = m_deque.back();
            m_deque.pop_back();
            Number * num = dynamic_cast<Number *>(op.get());
            if(num) {
                auto value = num->get_element_value();
                m_deque.push_back(make_shared<Number>(-value));
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void opEq() override {
        std::cout << "==" << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>("=="));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            String * str1 = dynamic_cast<String *>(op1.get());
            String * str2 = dynamic_cast<String *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                if(value1 == value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else if(str1 && str2) {
                string value1 = str1->get_element_value();
                string value2 = str2->get_element_value();
                if(value1 == value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void opNe() override {
        std::cout << "!=" << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>("!="));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            String * str1 = dynamic_cast<String *>(op1.get());
            String * str2 = dynamic_cast<String *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                if(value1 != value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else if(str1 && str2) {
                string value1 = str1->get_element_value();
                string value2 = str2->get_element_value();
                if(value1 != value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else {
                return;
            }
        } else {
            return;
        }

    }
    void opLt() override {
        std::cout << "<" << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>("<"));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            String * str1 = dynamic_cast<String *>(op1.get());
            String * str2 = dynamic_cast<String *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                if(value1 < value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else if(str1 && str2) {
                string value1 = str1->get_element_value();
                string value2 = str2->get_element_value();
                if(value1 < value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else {
                return;
            }
        } else {
            return;
        }

    }
    void opLe() override {
        std::cout << "<=" << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>("<="));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            String * str1 = dynamic_cast<String *>(op1.get());
            String * str2 = dynamic_cast<String *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                if(value1 <= value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else if(str1 && str2) {
                string value1 = str1->get_element_value();
                string value2 = str2->get_element_value();
                if(value1 <= value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void opGt() override {
        std::cout << ">" << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>(">"));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            String * str1 = dynamic_cast<String *>(op1.get());
            String * str2 = dynamic_cast<String *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                if(value1 > value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else if(str1 && str2) {
                string value1 = str1->get_element_value();
                string value2 = str2->get_element_value();
                if(value1 > value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void opGe() override {
        std::cout << ">=" << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<BinaryOperation>(">="));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            if(m_deque.size() < 2) {
                m_deque.pop_front();
                return;
            }
            shared_ptr<Element> op2 = m_deque.back();
            m_deque.pop_back();
            shared_ptr<Element> op1 = m_deque.back();
            m_deque.pop_back();
            Number * num1 = dynamic_cast<Number *>(op1.get());
            Number * num2 = dynamic_cast<Number *>(op2.get());
            String * str1 = dynamic_cast<String *>(op1.get());
            String * str2 = dynamic_cast<String *>(op2.get());
            if(num1 && num2) {
                double value1 = num1->get_element_value();
                double value2 = num2->get_element_value();
                if(value1 >= value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else if(str1 && str2) {
                string value1 = str1->get_element_value();
                string value2 = str2->get_element_value();
                if(value1 >= value2) {
                    m_deque.push_back(make_shared<Number>(1));
                } else {
                    m_deque.push_back(make_shared<Number>(0));
                }
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void valNumber(double val) override {
        std::cout << "Number: " << val << std::endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<Number>(val));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            m_deque.push_back(make_shared<Number>(val));
        } else {
            return;
        }
    }
    void valString(std::string val) override {
        cout << "String: " << val << endl;
        if(m_is_ref_deque) {
            m_ref_deque->push_back(make_shared<String>(val));
        }
        if(m_is_deque) {
            m_deque.pop_front();
            m_deque.push_back(make_shared<String>(val));
        } else {
            return;
        }
    }
    void valReference(std::string val) override {
        cout << "Reference: " << val << endl;
        CPos position;
        if(m_offset_column != 0 || m_offset_row != 0) {
            if(val.find("$") == 0 && val.rfind("$") > 0) {
                string val_clear = val;
                val_clear.erase(std::remove(val_clear.begin(), val_clear.end(), '$'), val_clear.end());
                CPos position_inner(val_clear);
                string new_pos_str = "$" + position_inner.get_column_str() + "$" + to_string(position_inner.get_row());
                position = position_inner;
                if(m_is_ref_deque) {
                    m_ref_deque->push_back(make_shared<Reference>(new_pos_str));
                }
            } else if(val.find("$") == 0) {
                string val_clear = val;
                val_clear.erase(std::remove(val_clear.begin(), val_clear.end(), '$'), val_clear.end());
                pair<int, int> coordinates = cell_parser_func(val_clear);
                int row_with_offset = coordinates.second + m_offset_row;
                CPos position_inner(coordinates.first, row_with_offset);
                string new_pos_str = "$" + position_inner.get_pos_str();
                position = position_inner;
                if(m_is_ref_deque) {
                    m_ref_deque->push_back(make_shared<Reference>(new_pos_str));
                }
            } else if(val.find("$") != std::string::npos) {
                string val_clear = val;
                val_clear.erase(std::remove(val_clear.begin(), val_clear.end(), '$'), val_clear.end());
                pair<int, int> coordinates = cell_parser_func(val_clear);
                int column_with_offset = coordinates.first + m_offset_column;
                CPos position_inner(column_with_offset, coordinates.second);
                string new_pos_str = position_inner.get_column_str() + "$" + to_string(position_inner.get_row());
                position = position_inner;
                if(m_is_ref_deque) {
                    m_ref_deque->push_back(make_shared<Reference>(new_pos_str));
                }
            } else {
                string val_clear = val;
                val_clear.erase(std::remove(val_clear.begin(), val_clear.end(), '$'), val_clear.end());
                pair<int, int> coordinates = cell_parser_func(val_clear);
                int column_with_offset = coordinates.first + m_offset_column;
                int row_with_offset = coordinates.second + m_offset_row;
                CPos position_inner(column_with_offset, row_with_offset);
                string new_pos_str = position_inner.get_pos_str();
                position = position_inner;
                if(m_is_ref_deque) {
                    m_ref_deque->push_back(make_shared<Reference>(new_pos_str));
                }
            }
        } else {
            string val_clear = val;
            val_clear.erase(std::remove(val_clear.begin(), val_clear.end(), '$'), val_clear.end());
            CPos position_inner(val_clear);
            position = position_inner;
            if(m_is_ref_deque) {
                m_ref_deque->push_back(make_shared<Reference>(val));
            }
        }
        if(!position.get_pos_str().empty() && m_is_deque) {
            m_deque.pop_front();
            auto cell = m_spreadsheet->getCell(position);
            if(cell != nullptr) {
                deque<shared_ptr<Element>> deque = cell->get_deque();
                if(deque.empty()) {
                    auto value = cell->get_cell_value();
                    auto value2 = cell->get_cell_value();
                    if(std::holds_alternative<double>(value)) {
                        m_deque.push_back(make_shared<Number>(std::get<double>(value)));
                    } else if(std::holds_alternative<std::string>(value2)) {
                        m_deque.push_back(make_shared<String>(std::get<std::string>(value2)));
                    } else {
                        m_deque.push_back(make_shared<EmptyElement>());
                    }
                } else {
                    MyExprBuilder builder(m_spreadsheet, ref_pos_str, deque);
                    for(auto & element: deque) {
                        element->apply(builder);
                    }
                    m_deque.push_back(builder.m_deque.back());
                }
            } else {
                return;
            }
        } else {
            return;
        }
    }
    void valRange(std::string val) override {};
    void funcCall(std::string fnName, int paramCount) override {};

    CSpreadsheet * m_spreadsheet;
    string ref_pos_str;
    deque<shared_ptr<Element>> m_deque;
    deque<shared_ptr<Element>> * m_ref_deque;
    bool m_is_ref_deque = false;
    bool m_is_deque = false;
    int m_offset_column = 0;
    int m_offset_row = 0;
};

void BinaryOperation::apply(MyExprBuilder & builder) const {
    if(operation == "+") {
        builder.opAdd();
    } else if(operation == "-") {
        builder.opSub();
    } else if(operation == "*") {
        builder.opMul();
    } else if(operation == "/") {
        builder.opDiv();
    } else if(operation == "^") {
        builder.opPow();
    } else if(operation == "==") {
        builder.opEq();
    } else if(operation == "!=") {
        builder.opNe();
    } else if(operation == "<") {
        builder.opLt();
    } else if(operation == "<=") {
        builder.opLe();
    } else if(operation == ">") {
        builder.opGt();
    } else if(operation == ">=") {
        builder.opGe();
    }
}

void UnaryOperation::apply(MyExprBuilder & builder) const {
    if(operation == "!") {
        builder.opNeg();
    }
}

void Number::apply(MyExprBuilder & builder) const {
    builder.valNumber(value);
}
void String::apply(MyExprBuilder & builder) const {
    builder.valString(value);
}

void Reference::apply(MyExprBuilder & builder) const {
    builder.valReference(value);
}

bool CSpreadsheet::setCell(CPos pos, std::string contents) {
    try {
        std::istringstream iss(contents);
        double number;
        if (iss >> number && iss.eof()) {
            auto cell = make_shared<MyNumber>(pos, number, deque<shared_ptr<Element>>());
            m_spreedsheet[pos] = cell;
        } else {
            if(contents[0] == '=') {
                string pos_str = pos.get_pos_str();
                deque<shared_ptr<Element>> deque;
                MyExprBuilder builder(this, pos_str, & deque);
                if(contents.size() > 1) {
                    parseExpression(contents, builder);
                }
                if(deque.empty()) {
                    auto cell = make_shared<MyEmpty>(pos, std::monostate(), std::deque<shared_ptr<Element>>());
                    m_spreedsheet[pos] = cell;
                    return true;
                } else {
                    vector<string> dependences_list;
                    for(const auto & element: deque) {
                        Reference * ref = dynamic_cast<Reference *>(element.get());
                        if(ref) {
                            string ref_pos_str = ref->get_element_value();
                            dependences_list.push_back(ref_pos_str);
                        }
                    }
                    m_dependences_list[pos_str] = dependences_list;
                    auto cell = make_shared<MyEmpty>(pos, std::monostate(), deque);
                    m_spreedsheet[pos] = cell;
                    return true;
                }
            } else {
                auto cell = make_shared<MyText>(pos, contents, deque<shared_ptr<Element>>());
                m_spreedsheet[pos] = cell;
                return true;
            }
        }
        return true;
    } catch (const std::exception & error) {
        cerr << "Error in setCell" << endl;
        return false;
    }
    return true;
}

bool CSpreadsheet::dfs_check(const std::string & node) {
    if(!visited[node]) {
        visited[node] = true;
        current[node] = true;

        for(auto & cell : m_dependences_list[node]) {
            if(!visited[cell] && dfs_check(cell))
                return true;
            else if (current[cell])
                return true;
        }
    }
    current[node] = false;
    return false;
}

bool CSpreadsheet::is_cyclic(const std::string & node) {
    if(dfs_check(node)) {
        return true;
    }
    return false;
}

CValue                             CSpreadsheet::getValue                                ( CPos                                  pos ) {
    auto it = m_spreedsheet.find(pos);
    if(it == m_spreedsheet.end()) {
        return CValue();
    }
    auto cell = it->second;
    string pos_str = pos.get_pos_str();
    deque<shared_ptr<Element>> deque = cell->get_deque();
    if(deque.empty()) {
        if(std::holds_alternative<double>(cell->get_cell_value())) {
            double value = std::get<double>(cell->get_cell_value());
            return CValue(value);
        } else if(std::holds_alternative<std::string>(cell->get_cell_value())) {
            string value = std::get<std::string>(cell->get_cell_value());
            if(value.empty()) {
                return CValue("");
            } else {
                return CValue(value);
            }
        } else {
            return CValue();
        }
    } else {
        current.clear();
        visited.clear();
        if(is_cyclic(pos_str)) {
            cout << "Cycle" << endl;
            return CValue();
        }
        MyExprBuilder builder(this, pos_str, deque);
        for(auto & element: deque) {
            element->apply(builder);
        }
        if(builder.m_deque.empty()) {
            return CValue();
        } else {
            Number * num = dynamic_cast<Number *>(builder.m_deque.back().get());
            String * str = dynamic_cast<String *>(builder.m_deque.back().get());
            if(num) {
                double value = num->get_element_value();
                return CValue(value);
            } else if(str) {
                string value = str->get_element_value();
                if(value.empty()) {
                    return CValue("");
                } else {
                    return CValue(value);
                }
            } else {
                return CValue();
            }
        }
    }
    return CValue();
}

std::shared_ptr<MyCell> CSpreadsheet::getCell(const CPos & pos) {
    auto it = m_spreedsheet.find(pos);
    if(it == m_spreedsheet.end()) {
        return nullptr;
    }
    auto cell = it->second;
    return cell;
}

void                               CSpreadsheet::copyRect                                ( CPos                                  dst,
                                                                                           CPos                                  src,
                                                                                           int                                   w,
                                                                                           int                                   h ) {
    map<CPos, shared_ptr<MyCell>> temp_storage;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            CPos pos_src(src.get_column() + x, src.get_row() + y);
            CPos pos_dst(dst.get_column() + x, dst.get_row() + y);
            if (m_spreedsheet.find(pos_src) != m_spreedsheet.end()) {
                shared_ptr<MyCell> cell = m_spreedsheet[pos_src]->clone();
                int offset_column = dst.get_column() - src.get_column();
                int offset_row = dst.get_row() - src.get_row();
                deque<shared_ptr<Element>> deque = cell->get_deque();
                if (deque.empty()) {
                    temp_storage[pos_dst] = cell;
                } else {
                    std::deque<shared_ptr<Element>> new_deque;
                    MyExprBuilder builder(this, pos_dst.get_pos_str(), deque, &new_deque, offset_column, offset_row);
                    if (!deque.empty()) {
                        for (auto &element: deque) {
                            element->apply(builder);
                        }
                    }
                    if (new_deque.empty()) {
                        auto cell = std::make_shared<MyEmpty>(pos_dst, std::monostate(),std::deque<shared_ptr<Element>>());
                        temp_storage[pos_dst] = cell;
                    } else {
                        auto cell = std::make_shared<MyEmpty>(pos_dst, std::monostate(), new_deque);
                        temp_storage[pos_dst] = cell;
                    }
                }

            }
        }
    }
    for (auto & [pos, new_cell]: temp_storage) {
        m_spreedsheet[pos] = new_cell;
    }
}

std::string convert_string(const std::string & input) {
    std::ostringstream ss;
    for(auto iter = input.begin(); iter != input.end(); ++iter) {
        switch(* iter) {
            case '\\': ss << "\\\\"; break;
            case '"':  ss << "\\\""; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default:   ss << * iter; break;
        }
    }
    return ss.str();
}

bool                               CSpreadsheet::save                                    ( std::ostream                        & os ) const {
     cout << "START LINE" << endl; cout << "START LINE" << endl; cout << "START LINE" << endl; cout << "START LINE" << endl;
    os << m_spreedsheet.size();
    int hashsum = 0;
    string procent = "%%%";
    for(char c: to_string(m_spreedsheet.size())) {
         hashsum += static_cast<int>(c);
    }
    for(char c: procent) {
        hashsum += static_cast<int>(c);
    }
    string resetka = "###";
    os << "%%%";
    for(char c: resetka) {
        hashsum += static_cast<int>(c);
    }
    string stolb = "[[[";
    os << "###";
    os << "[[[";
    for(char c: stolb) {
        hashsum += static_cast<int>(c);
    }

    for(auto & [pos, cell]: m_spreedsheet) {
        os << pos.get_pos_str();
        for(char c: pos.get_pos_str()) {
        hashsum += static_cast<int>(c);
    }
        os << "[[[";
        for(char c: stolb) {
        hashsum += static_cast<int>(c);
    }
        if(std::holds_alternative<double>(cell->get_cell_value())) {
            double value = std::get<double>(cell->get_cell_value());
            string value_str = std::to_string(value);
            string stdr = to_string(value) ;
                        stdr.erase(stdr.find_last_not_of('0') + 1, std::string::npos);
    if (stdr.back() == '.') {
        stdr.erase(stdr.length() - 1);
    }
            os << value_str.size();
            os << "[[[";
            os << value;
            os << "[[[";
            for(char c: stolb) {
        hashsum += static_cast<int>(c);
    }
    for(char c: stolb) {
        hashsum += static_cast<int>(c);
    }
             for(char c: to_string(value_str.size())) {
        hashsum += static_cast<int>(c);
    }
     for(char c: stdr) {
        hashsum += static_cast<int>(c);
    }
    
        } else if(std::holds_alternative<std::string>(cell->get_cell_value())) {
            string value = std::get<std::string>(cell->get_cell_value());
            os << convert_string(value).size();
            os << "[[[";
            os << convert_string(value);
            os << "[[[";
            for(char c: stolb) {
        hashsum += static_cast<int>(c);
    }
    for(char c: stolb) {
        hashsum += static_cast<int>(c);
    }
     for(char c: to_string(convert_string(value).size())) {
        hashsum += static_cast<int>(c);
    }
     for(char c: convert_string(value)) {
        hashsum += static_cast<int>(c);
    }

        } else {
            deque<shared_ptr<Element>> deque = cell->get_deque();
            if(!deque.empty()) {
                os << deque.size();
                 for(char c: to_string(deque.size())) {
        hashsum += static_cast<int>(c);
    }
                os << "[[[";
                for(char c: stolb) {
        hashsum += static_cast<int>(c);
    }
    for(char c: stolb) {
        hashsum += static_cast<int>(c);
    }
        
    char symbolrav = '=';
    hashsum += static_cast<int>(symbolrav);
                os << "=";
                os << "[[[";
                for(auto & element: deque) {
                        cout << "DDD" << endl;
                    if(auto number = dynamic_cast<Number *>(element.get())) {
                        os << number->get_element_value();
                        string str = to_string(number->get_element_value()) ;
                        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    if (str.back() == '.') {
        str.erase(str.length() - 1);
    }
                        cout << to_string(number->get_element_value()) << "DFF" << endl;
                        for(char c: str) {
        hashsum += static_cast<int>(c);
    }
                    } else if(auto string = dynamic_cast<String *>(element.get())) {
                        os << convert_string(string->get_element_value());
                        for(char c: convert_string(string->get_element_value())) {
        hashsum += static_cast<int>(c);
    }
                    } else if(auto reference = dynamic_cast<Reference *>(element.get())) {
                        os << reference->get_element_value();
                        for(char c: reference->get_element_value()) {
        hashsum += static_cast<int>(c);
    }
                    } else if(auto binary_operation = dynamic_cast<BinaryOperation *>(element.get())) {
                        os << binary_operation->get_element_value();
                        for(char c: binary_operation->get_element_value()) {
        hashsum += static_cast<int>(c);
    }
                    } else if(auto unary_operation = dynamic_cast<UnaryOperation *>(element.get())) {
                        os << unary_operation->get_element_value();
                        for(char c: unary_operation->get_element_value()) {
        hashsum += static_cast<int>(c);
    }
                    }
                    os << "[[[";
                    
                    for(char c: stolb) {
        hashsum += static_cast<int>(c);
    }
                }
            }
        }
        os << "###";
         for(char c: resetka) {
        hashsum += static_cast<int>(c);
    }
    }
    os << "%%%";
    for(char c: procent) {
        hashsum += static_cast<int>(c);
    }
     os << "chechsum" << hashsum;
    return true;
}

std::string decode_string(const std::string &input) {
    std::ostringstream ss;
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '\\' && i + 1 < input.length()) {
            switch (input[i + 1]) {
                case '\\': ss << '\\'; break;
                case '"':  ss << '"'; break;
                case 'n':  ss << '\n'; break;
                case 'r':  ss << '\r'; break;
                case 't':  ss << '\t'; break;
                default:   ss << '\\' << input[i + 1]; break;
            }
            ++i;
        } else {
            ss << input[i];
        }
    }
    return ss.str();
}

std::vector<std::string> split(const std::string & str, const std::string & delim) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

bool                               CSpreadsheet::load                                    ( std::istream                        & is ) {
    std::string start_line((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

    size_t start = start_line.find("%%%");
    string start_line1 = start_line;
        string start_line2 = start_line;
    if(start == string::npos) {
        return false;
    }
    
        int checksum = 0;
    size_t checksum_pos = start_line.find("chechsum");
    if(checksum_pos == string::npos) {
     
        return false;
    }
    int dsdfs = -1;
    
    string chechsum_str = start_line.substr(checksum_pos);
     chechsum_str = chechsum_str.substr(8);

        start_line1 = start_line1.substr(checksum_pos + 8);

    try {
      
    
        dsdfs = std::stoi(start_line1);
    } catch(...) {
        cout << "FAIL";
        return false;
    }

    string chech_line = start_line2.substr(0, checksum_pos);

    for(char c: chech_line) {

        checksum += static_cast<int>(c);
    }
    if(dsdfs != checksum) {
        return false;
    }
          cout << "ssfsef";
    string spreadsheet_size_str = start_line.substr(0, start);
    long unsigned int spreadsheet_size;
    try {
        spreadsheet_size = std::stoi(spreadsheet_size_str);

    } catch (const std::invalid_argument & error) {
        return false;
    } catch (const std::out_of_range & error) {
        return false;
    }
    string line = start_line.substr(start + 3);
    size_t end = line.find("%%%");
    if(end == string::npos) {
        return false;
    }
    line = line.substr(0, end);
    cout << "LINE" << endl;
    cout << line << endl;
    vector<string> tokens = split(line, "###");
    for(auto & token: tokens) {
        cout << "TOKEN" << endl;
        cout << token << endl;
        vector<string> cell_token = split(token, "[[[");
        if(cell_token.size() < 3) {
            return false;
        }
        string pos_str = cell_token[0];
        cout << "POS STR" << endl;
        cout << pos_str << endl;
        CPos pos(pos_str);
        if(pos.get_pos_str() != pos_str || pos.get_pos_str().empty()) {
            return false;
        }
        long unsigned int value_size = -1;
        string value_size_str = cell_token[1];
        try {
            value_size = std::stoi(value_size_str);
            cout << "VALUE SIZE" << endl;
            cout << value_size << endl;
        } catch (const std::invalid_argument & error) {
            return false;
        } catch (const std::out_of_range & error) {
            return false;
        }
        cell_token.erase(cell_token.begin(), cell_token.begin() + 2);
        if(cell_token.size() == 1) {
            string value_str = cell_token[0];
            try {
                double value = std::stoi(value_str);
                cout << "VALUE" << endl;
                cout << value << endl;
                if(to_string(value).size() != value_size) {
                    return false;
                } else {
                    auto cell = make_shared<MyNumber>(pos, value, deque<shared_ptr<Element>>());
                    m_spreedsheet[pos] = cell;
                }
            } catch (...) {
                cout << "VALUE" << endl;
                cout << value_str << endl;
                if(value_str.size() != value_size) {
                    return false;
                } else {
                    cout << value_str << endl;
                    if(value_str.size() > 0) {
                        string newsre = decode_string(value_str);
                        cout << "DEC " << newsre << endl;
                        auto cell = make_shared<MyText>(pos, newsre, deque<shared_ptr<Element>>());
                        m_spreedsheet[pos] = cell;
                    } else {
                        auto cell = make_shared<MyText>(pos, value_str, deque<shared_ptr<Element>>());
                        m_spreedsheet[pos] = cell;
                    }
                }
            }
        } else if(cell_token[0] == "=") {
            deque<shared_ptr<Element>> deque;
            cell_token.erase(cell_token.begin());
            for(auto & element: cell_token) {
                cout << "ELEMENT" << endl;
                cout << element << endl;
                if(element == "==") {
                    deque.push_back(make_shared<BinaryOperation>("=="));
                } else if(element == "!=") {
                    deque.push_back(make_shared<BinaryOperation>("!="));
                } else if(element == "<") {
                    deque.push_back(make_shared<BinaryOperation>("<"));
                } else if(element == "<=") {
                    deque.push_back(make_shared<BinaryOperation>("<="));
                } else if(element == ">") {
                    deque.push_back(make_shared<BinaryOperation>(">"));
                } else if(element == ">=") {
                    deque.push_back(make_shared<BinaryOperation>(">="));
                } else if(element == "+") {
                    deque.push_back(make_shared<BinaryOperation>("+"));
                } else if(element == "-") {
                    deque.push_back(make_shared<BinaryOperation>("-"));
                } else if(element == "*") {
                    deque.push_back(make_shared<BinaryOperation>("*"));
                } else if(element == "/") {
                    deque.push_back(make_shared<BinaryOperation>("/"));
                } else if(element == "^") {
                    deque.push_back(make_shared<BinaryOperation>("^"));
                } else if(element == "!") {
                    deque.push_back(make_shared<UnaryOperation>("!"));
                } else {
                    try {
                        double value = std::stod(element);
                        deque.push_back(make_shared<Number>(value));
                    } catch(...) {
                        try {
                            string val_clear = element;
                            val_clear.erase(std::remove(val_clear.begin(), val_clear.end(), '$'), val_clear.end());
                            CPos ref(val_clear);
                            if(ref.get_pos_str().empty() || ref.get_pos_str() != val_clear) {
                                return false;
                            }
                            deque.push_back(make_shared<Reference>(element));
                        } catch(...) {
                              try {
                            cout << element << endl;
                            if(element.size() > 0) {
                                string newsre = decode_string(element);
                                cout << "DEC " << newsre << endl;
                                deque.push_back(make_shared<String>(newsre));
                            } else {
                                deque.push_back(make_shared<String>(element));
                            }
                              } catch(...) {
                                return false;
                              }
                        }
                    }
                }
            }
            if(deque.size() != value_size) {
                return false;
            }
            auto cell = make_shared<MyEmpty>(pos, std::monostate(), deque);
            m_spreedsheet[pos] = cell;
        } else {
            return false;
        }
    }
    if(m_spreedsheet.size() != spreadsheet_size) {
        return false;
    }
    return true;
}

#ifndef __PROGTEST__

bool                                   valueMatch                              ( const CValue                        & r,
                                                                                 const CValue                        & s )

{
    if ( r . index () != s . index () )
        return false;
    if ( r . index () == 0 )
        return true;
    if ( r . index () == 2 )
        return std::get<std::string> ( r ) == std::get<std::string> ( s );
    if ( std::isnan ( std::get<double> ( r ) ) && std::isnan ( std::get<double> ( s ) ) )
        return true;
    if ( std::isinf ( std::get<double> ( r ) ) && std::isinf ( std::get<double> ( s ) ) )
        return ( std::get<double> ( r ) < 0 && std::get<double> ( s ) < 0 )
               || ( std::get<double> ( r ) > 0 && std::get<double> ( s ) > 0 );
    return fabs ( std::get<double> ( r ) - std::get<double> ( s ) ) <= 1e8 * DBL_EPSILON * fabs ( std::get<double> ( r ) );
}
int main ()
{
    CSpreadsheet x0, x1;
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;
    assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
    assert ( x0 . setCell ( CPos ( "A2" ), "20.5" ) );
    assert ( x0 . setCell ( CPos ( "A3" ), "3e1" ) );
    assert ( x0 . setCell ( CPos ( "A4" ), "=40" ) );
    assert ( x0 . setCell ( CPos ( "A5" ), "=5e+1" ) );
    assert ( x0 . setCell ( CPos ( "A6" ), "raw text with any characters, including a quote \" or a newline\n" ) );
    assert ( x0 . setCell ( CPos ( "A7" ), "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\"" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A1" ) ), CValue ( 10.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue ( 20.5 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 30.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A4" ) ), CValue ( 40.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A5" ) ), CValue ( 50.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A6" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A7" ) ), CValue ( "quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++." ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A8" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "AAAA9999" ) ), CValue() ) );
    assert ( x0 . setCell ( CPos ( "B1" ), "=A1+A2*A3" ) );
    assert ( x0 . setCell ( CPos ( "B2" ), "= -A1 ^ 2 - A2 / 2   " ) );
    assert ( x0 . setCell ( CPos ( "B3" ), "= 2 ^ $A$1" ) );
    assert ( x0 . setCell ( CPos ( "B4" ), "=($A1+A$2)^2" ) );
    assert ( x0 . setCell ( CPos ( "B5" ), "=B1+B2+B3+B4" ) );
    assert ( x0 . setCell ( CPos ( "B6" ), "=B1+B2+B3+B4+B5" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 625.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -110.25 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 1024.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 930.25 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 2469.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 4938.0 ) ) );
    assert ( x0 . setCell ( CPos ( "A1" ), "12" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 627.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -154.25 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 1056.25 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 5625.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 11250.0 ) ) );
    x1 = x0;
    assert ( x0 . setCell ( CPos ( "A2" ), "100" ) );
    assert ( x1 . setCell ( CPos ( "A2" ), "=A3+A5+A4" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3612.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -204.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 17424.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 24928.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 49856.0 ) ) );
    oss . clear ();
    oss . str ( "" );
    assert ( x0 . save ( oss ) );
    data = oss . str ();
    iss . clear ();
    iss . str ( data );
    assert ( x1 . load ( iss ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
    assert ( x0 . setCell ( CPos ( "A3" ), "4e1" ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
    oss . clear ();
    oss . str ( "" );
    assert ( x0 . save ( oss ) );
    data = oss . str ();
    for ( size_t i = 0; i < std::min<size_t> ( data . length (), 10 ); i ++ )
        data[i] ^=0x5a;
    iss . clear ();
    iss . str ( data );
    assert ( ! x1 . load ( iss ) );
    assert ( x0 . setCell ( CPos ( "D0" ), "10" ) );
    assert ( x0 . setCell ( CPos ( "D1" ), "20" ) );
    assert ( x0 . setCell ( CPos ( "D2" ), "30" ) );
    assert ( x0 . setCell ( CPos ( "D3" ), "40" ) );
    assert ( x0 . setCell ( CPos ( "D4" ), "50" ) );
    assert ( x0 . setCell ( CPos ( "E0" ), "60" ) );
    assert ( x0 . setCell ( CPos ( "E1" ), "70" ) );
    assert ( x0 . setCell ( CPos ( "E2" ), "80" ) );
    assert ( x0 . setCell ( CPos ( "E3" ), "90" ) );
    assert ( x0 . setCell ( CPos ( "E4" ), "100" ) );
    assert ( x0 . setCell ( CPos ( "F10" ), "=D0+5" ) );
    assert ( x0 . setCell ( CPos ( "F11" ), "=$D0+5" ) );
    assert ( x0 . setCell ( CPos ( "F12" ), "=D$0+5" ) );
    assert ( x0 . setCell ( CPos ( "F13" ), "=$D$0+5" ) );
    x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 1, 4 );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
    x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 2, 4 );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H10" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H11" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( 35.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue() ) );
    assert ( x0 . setCell ( CPos ( "F0" ), "-27" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );
    x0 . copyRect ( CPos ( "H12" ), CPos ( "H13" ), 1, 2 );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue ( 25.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( -22.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
