@echo off
echo ===== Building Food Order System =====

:: چک کردن g++
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: g++ not found!
    echo Please install MinGW from: https://www.mingw-w64.org/downloads/
    echo Or install MSYS2 from: https://www.msys2.org/
    pause
    exit /b 1
)

:: کامپایل مستقیم بدون cmake
g++ -std=c++17 -o FoodOrderSystem.exe ^
    main.cpp ^
    User.cpp ^
    Customer.cpp ^
    Admin.cpp ^
    RestaurantManager.cpp ^
    Restaurant.cpp ^
    Cart.cpp ^
    Order.cpp ^
    Item.cpp ^
    Food.cpp ^
    Drink.cpp ^
    OtherItem.cpp ^
    AppSystem.cpp ^
    Menus.cpp ^
    DataBaseManager.cpp ^
    UserDAO.cpp ^
    RestaurantDAO.cpp ^
    ItemDAO.cpp ^
    OrderDAO.cpp ^
    sqlite3.c ^
    -I. -lpthread

if %errorlevel% neq 0 (
    echo ERROR: Compilation failed.
    pause
    exit /b 1
)

echo ===== Build Successful! =====
echo.
echo Running program...
echo.
FoodOrderSystem.exe
pause