from django.db import models
import datetime

class Record(models.Model):
    temperature = models.FloatField()
    humidity = models.FloatField()
    pressure = models.FloatField()
    gas = models.FloatField()
    light = models.FloatField()
    date = models.DateTimeField(default = datetime.datetime.now)
