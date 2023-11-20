#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <iterator> // Содержит объявления категорий итераторов

using namespace std;
template <typename Type>
class SingleLinkedList {

   // Фиктивный узел, используется для вставки "перед первым элементом"
    struct Node;
    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        

    public:
      ///////////////////////////////////////////////////////////////////////////////////////////////////////
      //
      //
      // *******************ИТЕРАТОРЫ**КОНСТРУКТ ДЕСТРУКТ ПРИСВАИВАНИЕ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      //       Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора
      ////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

      ///////////////////////////////////////////////////////////////////////////////////////////////////////
      //
      //                     РЕАЛИЗАЦИЯ В МЕСТЕ ОБЬЯВЛЕНИЯ
      // *******************ИТЕРАТОРЫ**КОНСТРУКТ ДЕСТРУКТ ПРИСВАИВАНИЕ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      //   СЧИТАЮ ВЫНОС ДАННЫХ МЕТОДОВ НЕЦЕЛЕСООБРАЗНЫМ ВВИДУ КОРОТКОГО КОДА РЕАЛИЗАЦИИ
      ////////////////////////////////////////////////////////////////////////////////////////////////////////
        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        
     ///////////////////////////////////////////////////////////////////////////////////////////////////////
     //
     //                            РЕАЛИЗАЦИЯ В МЕСТЕ ОБЬЯВЛЕНИЯ
     // *****************************СРАВНЕНИЕ ИТЕРАТОРОВ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     //    СЧИТАЮ ВЫНОС ДАННЫХ МЕТОДОВ НЕЦЕЛЕСООБРАЗНЫМ ВВИДУ КОРОТКОГО КОДА РЕАЛИЗАЦИИ
     ////////////////////////////////////////////////////////////////////////////////////////////////////////
        
       // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return (this->node_ == rhs.node_);
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return (this->node_ != rhs.node_);
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return (this->node_ == rhs.node_);
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return (this->node_ != rhs.node_);
        }

     ///////////////////////////////////////////////////////////////////////////////////////////////////////
     //
     //                            РЕАЛИЗАЦИЯ В МЕСТЕ ОБЬЯВЛЕНИЯ
     // *****************************ОПЕРАЦИИ НАД ИТЕРАТОРАМИ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     //     СЧИТАЮ ВЫНОС ДАННЫХ МЕТОДОВ НЕЦЕЛЕСООБРАЗНЫМ ВВИДУ КОРОТКОГО КОДА РЕАЛИЗАЦИИ                        
     ////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            if (node_ != nullptr) {
                this->node_ = this->node_->next_node;
            }
            return *this;
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            if (node_ != nullptr) { 
                auto old_value(*this);
                ++(*this);  return old_value;
            }
            return *this;
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            return (this->node_)->value;
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            return &(this->node_)->value;
        }

    private:
        
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                            РЕАЛИЗАЦИЯ В МЕСТЕ ОБЬЯВЛЕНИЯ
    //  **************************ИТЕРАТОР***СЛУЖЕБНЫЕ ПРИВАТНЫЕ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //           СЧИТАЮ ВЫНОС ДАННЫХ МЕТОДОВ НЕЦЕЛЕСООБРАЗНЫМ ВВИДУ КОРОТКОГО КОДА РЕАЛИЗАЦИИ
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
        Node* node_ = nullptr;
        friend class SingleLinkedList;
        // Класс списка объявляется дружественным, чтобы из методов списка
                // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) {
            node_ = node;
        }
    };

public:
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
     //
     //                           
     // *****************************ОБЬЯВЛЕНИЕ ТИПОВ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     //   
     ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
     //
     //                        РЕАЛИЗАЦИЯ В МЕСТЕ ОБЬЯВЛЕНИЯ
     // *******************СПИСОК**КОНСТРУКТ ДЕСТРУКТ ПРИСВАИВАНИЕ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     //         СЧИТАЮ ВЫНОС ДАННЫХ МЕТОДОВ НЕЦЕЛЕСООБРАЗНЫМ ВВИДУ КОРОТКОГО КОДА РЕАЛИЗАЦИИ
     ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    SingleLinkedList(const std::initializer_list<Type> obj) {
       PushByIterators(obj.begin(), obj.end()); }

    SingleLinkedList<Type>(const SingleLinkedList<Type>& rhs) {
        PushByIterators(rhs.begin(), rhs.end());
    }
    
    SingleLinkedList() {
        size_ = 0;
    }

    ~SingleLinkedList() {
        Clear();
    }

    SingleLinkedList<Type>& operator= (const SingleLinkedList<Type>& rhs) {
        if (this == &rhs)return *this;
        SingleLinkedList<Type>tmp(rhs);
        swap(tmp);
        return *this;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                               РЕАЛИЗАЦИЯ В МЕСТЕ ОБЬЯВЛЕНИЯ
    //  *****************************ИТЕРАТОРЫ ОДНОСВЯЗНОГО СПИСКА***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //             СЧИТАЮ ВЫНОС ДАННЫХ МЕТОДОВ НЕЦЕЛЕСООБРАЗНЫМ ВВИДУ КОРОТКОГО КОДА РЕАЛИЗАЦИИ
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        const auto It = ConstIterator(head_.next_node);
        return It;
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        const auto It = ConstIterator(nullptr);
        return It;
    }

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
   // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {

        return Iterator(&head_);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
         return ConstIterator(const_cast<Node*>(&head_));
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
       return  cbefore_begin();
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                              РЕАЛИЗАЦИЯ СМ vvvvvvv
    //  *****************************СЛУЖЕБНЫЕ ПУБЛИЧНЫЕ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //Вставить элемент в начало
    void PushFront(const Type& value);
    // Возвращает количество элементов в списке
    size_t GetSize() const noexcept;
    // Сообщает, пустой ли список
    bool IsEmpty() const noexcept;
    //чистит список
    void Clear() noexcept;
    //меняет местами элементы
    void swap(SingleLinkedList& other) noexcept;
    //* Втавляет элемент, следующий за pos.
    //* Возвращает итератор на вставленный элемент
    Iterator InsertAfter(ConstIterator pos, const Type& value);
    
    void PopFront() noexcept;
    //* Удаляет элемент, следующий за pos.
    //* Возвращает итератор на элемент, следующий за удалённым
    Iterator EraseAfter(ConstIterator pos) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                               РЕАЛИЗАЦИЯ СМ vvvvvvv
    //  *****************************ОПЕРАТОРЫ СРАВНЕНИЯ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool operator==(const SingleLinkedList<Type>& rhs);
    bool operator!=(const SingleLinkedList<Type>& rhs);
    bool operator <(const SingleLinkedList<Type>& rhs);
    bool operator >(const SingleLinkedList<Type>& rhs);
    bool operator<=(const SingleLinkedList<Type>& rhs);
    bool operator>=(const SingleLinkedList<Type>& rhs);

private:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //
    //  *****************************СТРУКТУРЫ И ПЕРЕМЕННЫЕ СПИСКА***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    Node head_;
    size_t size_;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                             РЕАЛИЗАЦИЯ СМ vvvvvvv
    //  **************************СПИСОК***СЛУЖЕБНЫЕ ПРИВАТНЫЕ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //для того чтобы работал size в шаблонной фукцмм
    size_t size();
    //по итераторам создает создает обьект
    template<typename It>
    void PushByIterators(It b, It e);

};


template <typename Type>
static void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) {
    lhs.swap(rhs);
}

//ОПЕРАТОРЫ СРАВНЕНИЯ
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Type>
bool SingleLinkedList<Type>::operator>=(const SingleLinkedList<Type>& rhs) {
    if (*this > rhs || *this == rhs)return true;
    return false;
}

template <typename Type>
bool SingleLinkedList<Type>::operator<=(const SingleLinkedList<Type>& rhs) {
    if (*this < rhs || *this == rhs)return true;
    return false;
}

template <typename Type>
bool SingleLinkedList<Type>::operator >(const SingleLinkedList<Type>& rhs) {
    return !(std::lexicographical_compare(this->cbegin(), this->cend(), rhs.cbegin(), rhs.cend()));
}

template <typename Type>
bool SingleLinkedList<Type>::operator<(const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(this->cbegin(), this->cend(), rhs.cbegin(), rhs.cend());
}


template <typename Type>
bool SingleLinkedList<Type>::operator!=(const SingleLinkedList<Type>& rhs) {
    return !(std::equal(this->cbegin(), this->cend(), rhs.cbegin()));
}


template <typename Type>
bool SingleLinkedList<Type>::operator==(const SingleLinkedList<Type>& rhs) {
    return std::equal(this->cbegin(), this->cend(), rhs.cbegin());
}


    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //
    //  *****************************СЛУЖЕБНЫЕ ПУБЛИЧНЫЕ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename Type>
size_t SingleLinkedList<Type>::GetSize() const noexcept {
    return size_;
}

template <typename Type>
bool SingleLinkedList<Type>::IsEmpty() const noexcept {
    return (size_ == 0);
}

template <typename Type>
void SingleLinkedList<Type>::Clear() noexcept {
    while (head_.next_node != nullptr) {
        Node* temp = head_.next_node;
        head_.next_node = temp->next_node;
        delete temp;
        --size_;
    }
}

template <typename Type>
void SingleLinkedList<Type>::swap(SingleLinkedList& other) noexcept {
    auto adress_tmp = this->head_.next_node;
    this->head_.next_node = other.head_.next_node;
    other.head_.next_node = adress_tmp;
    std::swap(size_, other.size_);
}

template <typename Type>
void SingleLinkedList<Type>::PushFront(const Type& value)  {
    Node* front = new Node(value, head_.next_node);
    head_.next_node = front;
    ++size_;
}


template <typename Type>
SingleLinkedList<Type>::BasicIterator<Type> SingleLinkedList<Type>::InsertAfter(ConstIterator pos, const Type& value) {
    if (pos.node_ == nullptr) return Iterator(nullptr);
    pos.node_->next_node = new Node(value, pos.node_->next_node);
    ++size_;
    return Iterator(pos.node_->next_node);
}


template <typename Type>
SingleLinkedList<Type>::BasicIterator<Type> SingleLinkedList<Type>::EraseAfter(ConstIterator pos) noexcept {
    if (pos == this->before_begin()) {
        this->PopFront();
        return Iterator(head_.next_node);
    }

    auto adress = pos.node_->next_node->next_node;
    delete pos.node_->next_node;
    pos.node_->next_node = adress;
    return Iterator(adress);

}

template <typename Type>
void SingleLinkedList<Type>::PopFront() noexcept {
    if (size_ == 0)return;
    auto adress = head_.next_node->next_node;
    delete head_.next_node;
    head_.next_node = adress;
    size_--;
}
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //
    //  *****************************СЛУЖЕБНЫЕ ПРИВАТНЫЕ***>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Type>
template<typename It>
void SingleLinkedList<Type>::PushByIterators(It b, It e) {

   //элементы Pushfront в обратном направлении
    SingleLinkedList<Type> tmp;
    auto b_ = b;
    while (b_ != e) {
        tmp.PushFront(*b_);
        ++b_;
    }
    //элементы Pushfront в прямом направлении
    SingleLinkedList<Type> ttt;
    for (const auto& i : tmp) {
        ttt.PushFront(i);
    }
    swap(ttt);
}

template<typename Type>
size_t SingleLinkedList<Type>::size(){
    return GetSize();
}

void Test4() {
    struct DeletionSpy {
        ~DeletionSpy() {
            if (deletion_counter_ptr) {
                ++(*deletion_counter_ptr);
            }
        }
        int* deletion_counter_ptr = nullptr;
    };

    // Проверка PopFront
    {
        SingleLinkedList<int> numbers{3, 14, 15, 92, 6};
        numbers.PopFront();
        assert((numbers == SingleLinkedList<int>{14, 15, 92, 6}));

        SingleLinkedList<DeletionSpy> list;
        list.PushFront(DeletionSpy{});
        int deletion_counter = 0;
        list.begin()->deletion_counter_ptr = &deletion_counter;
        assert(deletion_counter == 0);
        list.PopFront();
        assert(deletion_counter == 1);
    }



    // Доступ к позиции, предшествующей begin
    {
        SingleLinkedList<int> empty_list;
        const auto& const_empty_list = empty_list;
        assert(empty_list.before_begin() == empty_list.cbefore_begin());
        assert(++empty_list.before_begin() == empty_list.begin());
        assert(++empty_list.cbefore_begin() == const_empty_list.begin());

        SingleLinkedList<int> numbers{1, 2, 3, 4};
        const auto& const_numbers = numbers;
        assert(numbers.before_begin() == numbers.cbefore_begin());
        assert(++numbers.before_begin() == numbers.begin());
        assert(++numbers.cbefore_begin() == const_numbers.begin());

    }

    // Вставка элемента после указанной позиции
    {  // Вставка в пустой список


        {
            SingleLinkedList<int> lst;
            const auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
            assert((lst == SingleLinkedList<int>{123}));
            assert(inserted_item_pos == lst.begin());
            assert(*inserted_item_pos == 123);
        }



        // Вставка в непустой список
        {
            SingleLinkedList<int> lst{1, 2, 3};
            auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);

            assert(inserted_item_pos == lst.begin());
            assert(inserted_item_pos != lst.end());
            assert(*inserted_item_pos == 123);
            assert((lst == SingleLinkedList<int>{123, 1, 2, 3}));

            inserted_item_pos = lst.InsertAfter(lst.begin(), 555);
            assert(++SingleLinkedList<int>::Iterator(lst.begin()) == inserted_item_pos);
            assert(*inserted_item_pos == 555);
            assert((lst == SingleLinkedList<int>{123, 555, 1, 2, 3}));
        };


    }

    // Удаление элементов после указанной позиции
    {
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto& const_lst = lst;
            const auto item_after_erased = lst.EraseAfter(const_lst.cbefore_begin());
            assert((lst == SingleLinkedList<int>{2, 3, 4}));
            assert(item_after_erased == lst.begin());
        }
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto item_after_erased = lst.EraseAfter(lst.cbegin());
            assert((lst == SingleLinkedList<int>{1, 3, 4}));
            assert(item_after_erased == (++lst.begin()));
        }
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto item_after_erased = lst.EraseAfter(++(++lst.cbegin()));
            assert((lst == SingleLinkedList<int>{1, 2, 3}));
            assert(item_after_erased == lst.end());
        }
        {
            SingleLinkedList<DeletionSpy> list{DeletionSpy{}, DeletionSpy{}, DeletionSpy{}};
            auto after_begin = ++list.begin();
            int deletion_counter = 0;
            after_begin->deletion_counter_ptr = &deletion_counter;
            assert(deletion_counter == 0u);
            list.EraseAfter(list.cbegin());
            assert(deletion_counter == 1u);
        }
    }
    

    // Вспомогательный класс, бросающий исключение после создания N-копии
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                }
                else {
                    --(*countdown_ptr);
                }
            }
        }
        // Присваивание элементов этого типа не требуется
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
        // Как только обнулится, конструктор копирования выбросит исключение
        int* countdown_ptr = nullptr;
    };
    
    // Проверка обеспечения строгой гарантии безопасности исключений
    {
        bool exception_was_thrown = false;
        for (int max_copy_counter = 10; max_copy_counter >= 0; --max_copy_counter) {
            SingleLinkedList<ThrowOnCopy> list{ThrowOnCopy{}, ThrowOnCopy{}, ThrowOnCopy{}};
            try {
                int copy_counter = max_copy_counter;
                list.InsertAfter(list.cbegin(), ThrowOnCopy(copy_counter));
                assert(list.GetSize() == 4u);
            }
            catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                assert(list.GetSize() == 3u);
                break;
            }
            
        }
        assert(exception_was_thrown);
    }
}

int main() {
    Test4();
   

}
