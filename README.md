 
 # Калькуляция и списание товаров
 
 ## Описание
 
 Данное приложение представляет собой desktop систему учета товаров на складе с графическим интерфейсом на базе **Qt6 (Widgets)**. Приложение позволяет вести справочник товаров, выполнять операции прихода/расхода, рассчитывать стоимость складских остатков, а также оформлять **списание** с фиксацией истории и формированием отчетов.
 
 ## Основной функционал
 
 - **Управление товарами**
   - Добавление, редактирование и удаление товаров
   - Атрибуты товара: `ID`, название, категория, количество, цена за единицу
   - Поиск по названию и фильтрация по категории
 - **Складские операции**
   - Приход/расход товара (изменение количества)
   - Контроль корректности количества (запрет отрицательных значений)
 - **Калькуляция**
   - Расчет общей стоимости позиции (количество × цена)
   - Расчет общей стоимости склада (inventory value)
 - **Списание**
   - Списание товара на указанное количество
   - Автоматический расчет стоимости списания
   - Ведение истории списаний и генерация отчета по списаниям
 - **Заказы и отчеты**
   - Создание/редактирование заказов
   - Просмотр отчета по продажам
   - Экспорт отчетов в текстовый файл
 - **Автосохранение**
   - Автоматическая загрузка данных при старте и сохранение при закрытии приложения
 
 ## Требования
 
 - **CMake** 3.16+
 - **Qt6**: `Core`, `Widgets`, `Charts`
 - **C++ компилятор** с поддержкой стандарта **C++20**
 
 ## Сборка и запуск
 
 1. Клонируйте репозиторий:
 
 ```bash
 git clone https://github.com/borya-chip/course_work.git
 cd course-work
 ```
 
 2. Создайте директорию для сборки:
 
 ```bash
 mkdir build
 cd build
 ```
 
 3. Настройте проект с помощью CMake:
 
 ```bash
 cmake ..
 ```
 
 4. Соберите проект:
 
 ```bash
 cmake --build .
 ```
 
 5. Запустите приложение:
 
 - Windows (пример):
 
 ```bash
 .\ProductCalculationSystems.exe
 ```
 
 - Linux/macOS (пример):
 
 ```bash
 ./ProductCalculationSystems
 ```
 
 ## Хранение данных
 
 Приложение сохраняет данные в пользовательскую директорию **AppData** (через `QStandardPaths::AppDataLocation`). Используются файлы:
 
 - `products.dat` — данные по товарам
 - `orders.dat` — данные по заказам
 - `writeoff.dat` — записи о списаниях
 - `inventory.dat` — бинарное автосохранение состояния склада
 
 ## Структура проекта
 
 ```
 course-work/
 ├── CMakeLists.txt
 ├── include/
 │   ├── entities/
 │   │   ├── AbstractProduct.h
 │   │   ├── Product.h
 │   │   ├── Order.h
 │   │   ├── OrderItem.h
 │   │   ├── ProductRepository.h
 │   │   └── ProductRepository.tpp
 │   ├── exceptions/
 │   │   └── Exceptions.h
 │   ├── managers/
 │   │   ├── FileManager.h
 │   │   ├── DatabaseManager.h
 │   │   ├── ProductDataStore.h
 │   │   ├── OrderDataStore.h
 │   │   ├── WriteOffDataStore.h
 │   │   └── WriteOffRecord.h
 │   ├── services/
 │   │   ├── InventoryService.h
 │   │   ├── InventoryAdjustmentService.h
 │   │   ├── ProductFilterService.h
 │   │   ├── ProductValidator.h
 │   │   ├── DiscountCalculator.h
 │   │   ├── OrderService.h
 │   │   ├── WriteOffCalculator.h
 │   │   └── WriteOffService.h
 │   └── ui/
 │       ├── MainWindow.h
 │       ├── models/
 │       │   └── ProductModel.h
 │       ├── delegates/
 │       │   └── ActionsDelegate.h
 │       └── dialogs/
 │           ├── ProductDialog.h
 │           ├── OrderDialog.h
 │           ├── AddProductToOrderDialog.h
 │           ├── WriteOffDialog.h
 │           ├── ReportDialog.h
 │           ├── SalesReportDialog.h
 │           └── InventoryDialog.h
 └── src/
     ├── main.cpp
     ├── entities/
     ├── managers/
     ├── services/
     └── ui/
         ├── models/
         ├── delegates/
         └── dialogs/
 ```
