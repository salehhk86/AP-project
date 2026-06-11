@echo off
cd /d "%~dp0"
echo ===== Building Food Order System =====

if not exist build mkdir build

echo Compiling sqlite3.c with gcc...
gcc -c src\models\sqlite3.c -o build\sqlite3.o
if %errorlevel% neq 0 (
    echo ERROR: sqlite3 compilation failed.
    pause & exit /b 1
)

echo Compiling project with g++...
g++ -std=c++17 -o FoodOrderSystem.exe ^
    main\main.cpp ^
    main\Menus.cpp ^
    src\models\User.cpp ^
    src\models\Customer.cpp ^
    src\models\Admin.cpp ^
    src\models\RestaurantManager.cpp ^
    src\models\Restaurant.cpp ^
    src\models\Cart.cpp ^
    src\models\Order.cpp ^
    src\models\Item.cpp ^
    src\models\Food.cpp ^
    src\models\Drink.cpp ^
    src\models\OtherItem.cpp ^
    src\models\AppSystem.cpp ^
    src\models\DataBaseManager.cpp ^
    src\dao\ItemDAO.cpp ^
    src\dao\OrderDAO.cpp ^
    src\dao\RestaurantDAO.cpp ^
    src\dao\UserDAO.cpp ^
    build\sqlite3.o ^
    -I include\models ^
    -I include\dao ^
    -I main

if %errorlevel% neq 0 (
    echo ERROR: Compilation failed.
    pause & exit /b 1
)

echo ===== Build Successful! =====
FoodOrderSystem.exe
pause