# SP_GMod
### Скрипты для работы [Metrostroi](https://github.com/metrostroi-repo/MetrostroiAddon) с тренажером 81-717 (ООО "ПО "Зарница").

Библиотека работает с клиентом и не требует значительных модификаций на серверной стороне.

**`training_scripts`** - Папка аддона для Garry's Mod.  
**`uart_config`** - Папка для конфигурационных файлов. Хранится рядом с `gmod.exe`.  
**`GM_Zarnitsa`** - проект Premake с библиотекой Garry's Mod для работы с тренажером.  
**`Firmwares`** - дамп прошивок Universal CAN Controller v3.2 (STM32F103C8T6), в названии файла указан фактический порядковый номер контроллера. 

Использована библиотека [utf8_to_cp1251](https://github.com/bravikov/utf8_to_cp1251) (Распространяется по лицензии MIT)

# Инициализация проекта
1. Скачать [Premake5](https://premake.github.io/download/)
2. Положить бинарный файл рядом с `premake5.lua`
3. Выполнить команду в папке проекта:
```
premake5 vs2022
```
- ~~Для Linux вместо `vs2022` необходимо ввести `gmake`~~ Поддержка Linux отсутствует (пока что).

# Команды для работы с модулем TrainSignals
**Все команды выполняются в консоли клиента.**

`zarn_start [НОМЕР ПОРТА]` - Запуск работы модуля.  
`zarn_stop` - Остановка работы модуля.  
`zarn_force_stop` - Принудительная остановка работы модуля без деконфигурации.  
`zarn_reload_calibrations` - Перезагрузка калиборовок из файла `calibrations.ini`.  
`zarn_reload_sleeps` - Перезагрузка значений из файла `sleep.ini`.  

# Использование модуля-загрузчика (для клиента)
Загрузчик позволяет использовать тренажёр на сторонних серверах Metrostroi, минуя установку скриптов на серверной стороне.  
Модуль можно загрузить как через require, так и через инжект DLL напрямую.  

Установка загрузчика:
1. Скопировать `gmsv_trainsignals_loader_[OS].dll` в `garrysmod/lua/bin`
2. Скопировать `training_scripts/lua/metrostroi` в `garrysmod/lua/metrostroi` **(не в `garrysmod/addons`!)**
3. Добавить в конец скрипта `garrysmod/lua/menu/menu.lua` строчку `require("trainsignals_loader")`

После этого основной модуль автоматически загружается при заходе на любой сервер.

