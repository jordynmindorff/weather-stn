from django.shortcuts import render
from django.http import JsonResponse
from django.utils import timezone
import datetime
import json
from .models import Record

def current(request):
    if request.method == "GET":
        most_recent = Record.objects.all().order_by("-date")[:1].get()

        serialized_data = {
            "temperature": most_recent.temperature,
            "humidity": most_recent.humidity,
            "pressure": most_recent.pressure,
            "gas": most_recent.gas,
            "light": most_recent.light,
            "date": most_recent.date,
            "id": most_recent.id
        }

        return JsonResponse(serialized_data, safe = False)
    elif request.method == "POST":
        body = json.loads(request.body)

        if not "temperature" in body or not "humidity" in body or not "pressure" in body or not "gas" in body or not "light" in body:
           return JsonResponse({
                "success": False,
                "message": "Missing required parameters."
           }, status=400)

        good_data = {
            "temperature": body["temperature"],
            "humidity": body["humidity"],
            "pressure": body["pressure"],
            "gas": body["gas"],
            "light": body["light"]
        } # Just in case we get some random other fields sent in the request (so that we, preferably, avoid bricking the API)

        Record.objects.create(**good_data)

        return JsonResponse({
            "success": True
        }, status=201)
    else:
        return

def historical(request):
    if request.method != "GET":
        return
    
    days = int(request.GET.get('days', None)) # URL Query Param
    daysAgo = timezone.now() - datetime.timedelta(days=days)

    
    list = Record.objects.filter(date__gte=daysAgo).order_by("-date")

    serialized_data = []

    for r in list:
        serialized_data.append({
            "temperature": r.temperature,
            "humidity": r.humidity,
            "pressure": r.pressure,
            "gas": r.gas,
            "light": r.light,
            "date": r.date,
            "id": r.id
        })

    return JsonResponse(serialized_data, safe=False)
