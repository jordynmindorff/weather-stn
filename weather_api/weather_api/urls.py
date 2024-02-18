from django.urls import path, include

handler404 = 'weather_api.views.handler404'
handler500 = 'weather_api.views.handler500'

urlpatterns = [
    path("weather/", include("weather.urls")),
]
