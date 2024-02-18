from django.urls import path

from . import views

urlpatterns = [
    path('current/', views.current, name="current"),
    path('historical/', views.historical, name="historical") # Also Expecting URL Query Param "days"
]