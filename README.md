1 . git clone https://github.com/vahramkhachtrayan/weatherservie_bot.git
2. export LD_LIBRARY_PATH=$(pwd)/telebot/lib:$(pwd)/rust_weather/target/release
3. make clean
4. make
5. ./weather_bot <arg1> <arg2>
