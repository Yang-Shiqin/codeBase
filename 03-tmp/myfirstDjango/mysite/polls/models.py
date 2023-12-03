from django.db import models
from django.utils import timezone

# Create your models here.
class Question(models.Model):
    question_text = models.CharField(max_length=200)    # 问题描述，str
    pub_date = models.DateTimeField("date published")   # 发布日期，date

    def __str__(self):  # 介绍对象
        return self.question_text

    def was_published_recently(self):
        return self.pub_date >= timezone.now() - datetime.timedelta(days=1)

class Choice(models.Model):
    question = models.ForeignKey(Question, on_delete=models.CASCADE)    # 每个选项属于一个问题(数据库关系)
    choice_text = models.CharField(max_length=200)      # 选项描述，str
    votes = models.IntegerField(default=0)              # 当前得票数，int
    
    def __str__(self):
        return self.choice_text
