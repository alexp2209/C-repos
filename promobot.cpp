/*
Требуется реализовать программу, которая упорядочит массив объектов:
1) Создается несколько потоков, задача каждого - упорядочить один и тот же массив 
(берет единый список - упорядочивает его - выводит в консоль упорядоченный массив). 
Разница в потоках - разный алгоритм сортировки.
Т.е. вывод в консоль должен быть примерно такой:
> Сортировка: быстрая
> Результат: 1,2,3,4
> Сортировка: пузырьком
> Результат: 1,2,3,4
2) Массив должен состоять из объектов "геометрические фигуры" (круг, треугольник, прямоугольник). 
Реализовать классы требуется через наследование от абстрактного класса и реализовать в каждом метод расчета площади. 
Сортировку массива осуществить по этому показателю (по площади)
Формат сдачи - ссылка на github-проект. Высылать до 7 февраля на email intern@promo-bot.ru

виды сортировок:
1. Пузырьковая сортировка
3. Сортировка вставками
4. Сортировка выбором
5. Быстрая сортировка (QuickSort)

реализовать абстракт.класс с вирт методом рассчета площади
реализовать наследников от абстр.кл. с методом рассчета площади
создать массив наследников
создать 3-4 потока с разными алг сортировки по возрастанию площади
вывод результатов в консоль

*/

#include <iostream>
#include <vector>
#include <thread>
#include <cmath>
#include <mutex>

using namespace std;
mutex sortArray_mutex;

class abstractGeometricFigure
{
public:

    virtual float squareArea(float numbers1, float numbers2) = 0;
    
};

class Square : public abstractGeometricFigure
{
    private:
        int width;

    public:

	    Square(int new_width)
        {
            width = new_width;
        }

        float squareArea(float numbers1, float numbers2)override
        {;
            return width * width;
        }
        void print()
        {
            cout << "squareArea Square " << squareArea(0, 0) << endl;
        }
};

class Circle : public abstractGeometricFigure
{
    private:
        int radius;
        const float PI = 3.14159;

    public:
        Circle(int new_radius)
        {
            radius = new_radius;
        }

        float squareArea(float numbers1, float numbers2)override
        {
            return radius * radius * PI;
        }
        void print()
        {
            cout << "squareArea Circle " << squareArea(0, 0) << endl;
        }
};


class Triangle : public abstractGeometricFigure
{
    private:
        int base;
        int height;
    public:
        Triangle(int new_base, int new_height)
        {
            base = new_base;
            height = new_height;
        }
        float squareArea(float numbers1, float numbers2)override
        {
            return static_cast<float>(base * height) * 0.5;
        }
        void print()
        {
            cout << "squareArea Triangle " << squareArea(0, 0) << endl;
        }
};


class Rectangle : public abstractGeometricFigure
{
    private:
        int width;
        int height;
    public:
        Rectangle(int new_width, int new_height)
        {
            width = new_width;
            height = new_height;
        }
        float squareArea(float numbers1, float numbers2)override
        {
            return static_cast<float>(width * height);
        }
        void print()
        {
            cout << "squareArea Rectangle " << squareArea(0, 0) << endl;
        }
};


void displayArray(abstractGeometricFigure** array, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << "Result sortArea:  " << array[i]->squareArea(0, 0) << endl;
    }
    cout << endl;
}


bool compareFiguresByArea( abstractGeometricFigure* a,  abstractGeometricFigure* b) {
    return a->squareArea(0, 0) < b->squareArea(0, 0);
}

void swaped(abstractGeometricFigure* a, abstractGeometricFigure* b) {
    abstractGeometricFigure* temp = a;
    a = b;
    b = temp;
}

void quickSort(abstractGeometricFigure** figures, int left, int right, bool(*compare)( abstractGeometricFigure*, abstractGeometricFigure*))
{   
    
    if (left < right) {
        int pivot = left;
        for (int i = left + 1; i <= right; ++i) 
        {
            if (compare(figures[i], figures[left]))
            {
                pivot++;
                swaped(figures[i], figures[pivot]);
            }
        }
        swaped(figures[left], figures[pivot]);
        quickSort(figures, left, pivot - 1, compare);
        quickSort(figures, pivot + 1, right, compare);
    }
    
}
void quickSortDaughter(abstractGeometricFigure** figures, int left, int right, bool(*compare)(abstractGeometricFigure*, abstractGeometricFigure*),int size)
{
    lock_guard<mutex> lock(sortArray_mutex); 
    quickSort(figures, left, right, compare);
    cout << "quickSort" << endl;
    displayArray(figures, size);
    cout << "ID thread end = " << this_thread::get_id() << endl << endl;
}

void bubbleSort(abstractGeometricFigure** figures,int size)
{
    lock_guard<mutex> lock(sortArray_mutex);
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {

            if (figures[j]->squareArea(0, 0) > figures[j + 1]->squareArea(0, 0))
            {
                abstractGeometricFigure* temp = figures[j];
                figures[j] = figures[j + 1];
                figures[j + 1] = temp;
            }
        }
    }
    cout << "bubleSort" << endl;
    displayArray(figures, size);
    cout << "ID thread end = " << this_thread::get_id() << endl << endl;
}

void insertSort(abstractGeometricFigure** figures, int size)
{
    lock_guard<mutex> lock(sortArray_mutex);
    for (int i = 1; i < size; i++)
    {
        int key = figures[i]->squareArea(0, 0);
        int j = i - 1;
        abstractGeometricFigure* key_copy = figures[i];

        while (j >= 0 && figures[j]->squareArea(0, 0) > key) {
            figures[j + 1] = figures[j];
            j--;
        }
        figures[j + 1] = key_copy;

    }
    cout << "insertSort" << endl;
    displayArray(figures, size);
    cout << "ID thread end = " << this_thread::get_id() << endl << endl;
}

int main()
{
    abstractGeometricFigure* figures[4];

    figures[0] = new Square(5);
    figures[1] = new Circle(4);
    figures[2] = new Triangle(3, 2);
    figures[3] = new Rectangle(3, 5);


    Square* squared = dynamic_cast<Square*>(figures[0]);
        squared->print();

    Circle* circle = dynamic_cast<Circle*>(figures[1]);
        circle->print();

    Triangle* triangle = dynamic_cast<Triangle*>(figures[2]);
        triangle->print();

    Rectangle* rect = dynamic_cast<Rectangle*>(figures[3]);
        rect->print();

        cout << endl;
        

    //Size Figures 
    int size = sizeof(figures) / sizeof(figures[0]);
    
    thread first(bubbleSort, figures, size);

    thread second(insertSort, figures, size);

    thread third(quickSortDaughter,figures, 0, size - 1, compareFiguresByArea, size);
    


    first.join();
    second.join();
    third.join();
    
    
    return 0;
}

