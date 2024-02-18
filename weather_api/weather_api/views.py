from django.http import JsonResponse

def handler404(request, exception):
    return JsonResponse({
        "success": False,
        "message": "Route not found for the specified HTTP method."
    }, status=404)

def handler500(request):
    return JsonResponse({
        "success": False,
        "message": "Internal server error. Maybe no records were found."
    }, status=500)