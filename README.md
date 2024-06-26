# База данных бегунов
Данный проект демонстрирует работу 2-х частей: __база данных__(Parts/DB), __прасер__(Parts/Parser), __сервер__(Parts/Server)
## Установка и компиляция 
Для работы программы требуются следующие библиотеки: bison и flex. Для сборки используется cmake и make. В дистрибутиве ubuntu установка такова:
```
sudo apt-get install flex bison cmake libboost-all-dev
```
Далее программу нужно собрать. Перейдите в директорию с проектом и выполните:
```
mkdir build && cd build
cmake ..
make
```
## Части(Parts)
### База данных(DB)
В качестве базы данных я использую базу данных бегунов. Для хранения используется формат __CSV__, данные загружаются в память и хранятся в объекте __std::set__, который представляет из себя самобалансирующееся дерево.  Для хранения данных используется объект Runner, с такими полями:
1) Имя(std::string)(name)
2) Фамилия(std::string)(surname)
3) Отчество(std::string)(patronymic)
4) Пол(std::string М/Ж)(gender)
5) Возраст(int [18, 60])(age)
6) Стартовый номер(int)(number)
7) Город(std::string)(city)
8) Время(int [7800, 21600])(sec)

Для генерации примера базы данных написан генератор. Для запуска используйте команду:
```
./generator runners.csv
```
или:
```
./generator runners.csv 1000
```
если хотите задать число записей

### Парсер(Parser)
Парсер выполнен с использованием программ __flex__ и __bison__ и способен распозновать корректные для него скрипты. Скрипт состоит из команд, разделенных точками с запятой. Каждая команда начинается с операции (__select, insert, delete__), далее идут параметры (__name, surname, patronymic, gender, age, number, city, sec__) со своими значениями указанными через "__=__". в конце запроса ставится "__;__" То есть для того, чтобы получить всех бегунов 30 лет, нужно написать:
```
select age = 30;
```
Пробельные символы игнорируются. Для того, чтобы вывести всех, вместо параметров указывается ключевое слово __all__:
```
select all;
```
__Insert__ требует всех параметров, в связи с чем __insert all__; работать не будет. __Delete__ работает аналогично __select__.
```
delete all;
delete age = 30 name = Илья;
```

Для того, чтобы запустить парсер, используйте команду:
```
./parser runners.csv test_simple.txt
```
где test_simple.dbq представляется из скрипт с запросами вроде тех, что фигурируют выше

### Сервер(Server)

Проект представляет собой простой сервер, работающий с клиентами через сокеты __TCP/IP__. Сервер реализован с использованием __сокетов__ для обмена данными с клиентами. Сервер является асинхронным и способен поддерживать несколько подключений, в частности поддерживает паралельное подключение некоторого числа клиентов, оставшиеся в таком случае добавляются в очередь

**Системные вызовы**:
   - `socket`: Создание сокета для обмена данными.
   - `bind`: Привязка сокета к определенному адресу и порту.
   - `listen`: Прослушивание входящих соединений на серверном сокете.
   - `accept`: Принятие входящего соединения от клиента.
   - `connect`: Установка соединения с сервером.
   - `send` и `recv`: Отправка и прием данных через сокет.

#### Как запустить сервер с клиента
1. **Запуск сервера**:
   - чтобы запустить сервер, укажите параметры командной строки: `./server [DBfile] [port] [max_clients]`, где
     - `[DBfile]` - путь к файлу базы данных.
     - `[port]` - порт, на котором сервер будет прослушивать входящие соединения.
     - `[max_clients]` - максимальное количество одновременно подключенных клиентов.

например:
```
./server runners.csv 4444
```

2. **Запуск клиента**:
   - Затем в новом окне запустите клиент, указав параметры командной строки: `./client [host] [port]`, где
     - `[host]` - IP-адрес или доменное имя сервера.
     - `[port]` - порт, на котором сервер прослушивает входящие соединения.

например:
```
./client 127.0.0.1 4444
```

В клиент можно сразу передать файл скрипта:
```
./client 127.0.0.1 4444 < test_simple.dbq
```

3. **Взаимодействие с сервером**:
   - После успешного запуска клиента вы сможете взаимодействовать с сервером, отправляя ему запросы и получая ответы.
   - Для завершения работы с сервером введите `stop` в клиентском приложении.



### HTTP

