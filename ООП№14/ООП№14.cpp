/*Задание 

Создать игру, где будут 2-3 барана и волк. 
При совпадении координат волка с бараном баран исчезает. 
При совпадении координат баранов появляется новый баран. 
Все движутся хаотически. Поле  - двумерный массив. 
Вывод на экран состояния игрового поля по нажатию клавиши.*/
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h> // Для функции _kbhit() и _getch()

const int FIELD_SIZE = 10;

struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// Базовый класс для сущностей (волк и бараны)
class Entity {
protected:
    Position pos;

public:
    Entity(int x, int y) : pos({ x, y }) {}

    Position getPosition() const {
        return pos;
    }

    virtual void move() {
        int direction = rand() % 4;
        switch (direction) {
        case 0: pos.x = (pos.x + 1) % FIELD_SIZE; break; // Вниз
        case 1: pos.x = (pos.x - 1 + FIELD_SIZE) % FIELD_SIZE; break; // Вверх
        case 2: pos.y = (pos.y + 1) % FIELD_SIZE; break; // Вправо
        case 3: pos.y = (pos.y - 1 + FIELD_SIZE) % FIELD_SIZE; break; // Влево
        }
    }

    virtual char getSymbol() const = 0;
};

// Класс Баран
class Sheep : public Entity {
public:
    Sheep(int x, int y) : Entity(x, y) {}

    char getSymbol() const override {
        return 'Б'; // Символ для отображения барана
    }
};

// Класс Волк
class Wolf : public Entity {
public:
    Wolf(int x, int y) : Entity(x, y) {}

    char getSymbol() const override {
        return 'В'; // Символ для отображения волка
    }
};

// Класс Игровое поле
class GameField {
private:
    std::vector<Sheep> sheeps;
    Wolf wolf;

public:
    GameField(int sheepCount) : wolf(rand() % FIELD_SIZE, rand() % FIELD_SIZE) {
        // Инициализация баранов
        for (int i = 0; i < sheepCount; ++i) {
            sheeps.emplace_back(rand() % FIELD_SIZE, rand() % FIELD_SIZE);
        }
    }

    // Отображение игрового поля
    void displayField() const {
        char field[FIELD_SIZE][FIELD_SIZE] = { {'.'} };

        // Расставляем баранов
        for (const Sheep& sheep : sheeps) {
            field[sheep.getPosition().x][sheep.getPosition().y] = sheep.getSymbol();
        }

        // Расставляем волка
        field[wolf.getPosition().x][wolf.getPosition().y] = wolf.getSymbol();

        // Вывод поля
        for (int i = 0; i < FIELD_SIZE; ++i) {
            for (int j = 0; j < FIELD_SIZE; ++j) {
                std::cout << field[i][j] << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // Обновление состояния игры
    void update() {
        // Движение волка и баранов
        wolf.move();
        for (Sheep& sheep : sheeps) {
            sheep.move();
        }


        for (auto it = sheeps.begin(); it != sheeps.end();) {
            if (wolf.getPosition() == it->getPosition()) {
                it = sheeps.erase(it); // Волк съел барана
                std::cout << "Волк съел барана!" << std::endl;
            }
            else {
                ++it;
            }
        }

        // Проверка на совпадение координат баранов
        for (size_t i = 0; i < sheeps.size(); ++i) {
            for (size_t j = i + 1; j < sheeps.size(); ++j) {
                if (sheeps[i].getPosition() == sheeps[j].getPosition()) {
                    // Появляется новый баран
                    sheeps.emplace_back(rand() % FIELD_SIZE, rand() % FIELD_SIZE);
                    std::cout << "Два барана встретились! Новый баран появился." << std::endl;
                }
            }
        }
    }

    bool isGameOver() const {
        return sheeps.empty();
    }
};


int main() {
    setlocale(LC_ALL, "RU");
    srand(static_cast<unsigned int>(time(0))); 

    GameField field(3); 

    while (true) {
       
        field.displayField();

        // Проверка на завершение игры
        if (field.isGameOver()) {
            std::cout << "Игра окончена! Все бараны съедены." << std::endl;
            break;
        }

        std::cout << "Нажмите любую клавишу для следующего шага..." << std::endl;
        while (!_kbhit()) {} // Ждем нажатия клавиши
        _getch(); // Считываем нажатие клавиши

        field.update(); // Обновление состояния игры
    }

    return 0;
}
