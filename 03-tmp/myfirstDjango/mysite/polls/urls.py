#!/usr/bin/env python
# coding=utf-8
from django.urls import path
from . import views

app_name = "polls"  # 区分多个应用同名的模板中的url视图
urlpatterns = [
    path("", views.IndexView.as_view(), name="index"),
    path("<int:pk>/", views.DetailView.as_view(), name="detail"),
    path("<int:pk>/results/", views.ResultsView.as_view(), name="results"),
    path("<int:question_id>/vote/", views.vote, name="vote"),
]
