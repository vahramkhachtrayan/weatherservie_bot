use open_meteo_api::query::OpenMeteo;
use std::error::Error;
use std::ffi::{CStr, CString};
use std::ptr;
use tokio;  // Use Tokio to run async functions in a blocking way

#[unsafe(no_mangle)]
pub extern "C" fn get_weather_by_city(city: *const i8, api_key: *const i8, weather_info: *mut i8, weather_info_len: usize) -> i32 {
    // Convert C string to Rust string
    let c_city = unsafe { CStr::from_ptr(city) };
    let city_str = c_city.to_str().unwrap();

    let c_api_key = unsafe { CStr::from_ptr(api_key) };
    let api_key_str = c_api_key.to_str().unwrap();

    let runtime = tokio::runtime::Runtime::new().unwrap(); 
    let weather = runtime.block_on(fetch_weather_by_city(city_str, api_key_str)); 

    match weather {
        Ok(data) => {
            // Convert the weather information into a C string
            let weather_str = format!("Weather in {}: {}°C", city_str, data.temperature);
            let c_weather_info = CString::new(weather_str).unwrap();
            let c_weather_info_ptr = c_weather_info.as_ptr();

            unsafe {
                // Copy the result into the provided C buffer
                ptr::copy_nonoverlapping(
                    c_weather_info_ptr,
                    weather_info as *mut i8,
                    weather_info_len.min(c_weather_info.to_bytes().len()),
                );
            }

            0 
        }
        Err(_) => -1, 
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn get_weather_by_coordinates(latitude: f64, longitude: f64, weather_info: *mut i8, weather_info_len: usize) -> i32 {
    let runtime = tokio::runtime::Runtime::new().unwrap(); 
    let weather = runtime.block_on(fetch_weather_by_coordinates(latitude, longitude)); 

    match weather {
        Ok(data) => {
            let weather_str = format!(
                "Weather at lat: {}, lon: {}: {}°C", 
                latitude, longitude, data.temperature
            );
            let c_weather_info = CString::new(weather_str).unwrap();
            let c_weather_info_ptr = c_weather_info.as_ptr();

            unsafe {
                ptr::copy_nonoverlapping(
                    c_weather_info_ptr,
                    weather_info as *mut i8,
                    weather_info_len.min(c_weather_info.to_bytes().len()),
                );
            }

            0 
        }
        Err(_) => -1, 
    }
}

async fn fetch_weather_by_city(city: &str, api_key: &str) -> Result<WeatherData, Box<dyn Error>> {
    
    let data = OpenMeteo::new()
        .location(city, api_key) // api_key required for OpenMeteo release v0.1.4 (v0.1.3 yanked)
        .await?
        .current_weather()?
        .query()
        .await?;

    Ok(WeatherData {
        temperature: data.current_weather.unwrap().temperature,
    })
}


async fn fetch_weather_by_coordinates(
    latitude: f64,
    longitude: f64,
) -> Result<WeatherData, Box<dyn Error>> {

    let latitude_f32 = latitude as f32;
    let longitude_f32 = longitude as f32;

    let data = OpenMeteo::new()
        .coordinates(latitude_f32, longitude_f32)?
        .current_weather()?
        .query()
        .await?;

    Ok(WeatherData {
        temperature: data.current_weather.unwrap().temperature,
    })
}

struct WeatherData {
    temperature: f32,
}
