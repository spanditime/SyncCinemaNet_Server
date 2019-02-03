from django.shortcuts import render


"""___Your views is here.___"""


def index(request):
    return render(request, "index.html")
