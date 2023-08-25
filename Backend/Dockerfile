FROM python:3.9

WORKDIR /app

COPY requirements.txt .

RUN pip install --no-cache-dir -r requirements.txt

COPY . .

RUN apt-get update && apt-get -y install g++

RUN g++ -std=c++11 SurveyCS/SurveyCS/ktruss/online/src/main.cpp -o ktcom

RUN g++ -std=c++11 SurveyCS/SurveyCS/kcore/online/src/main.cpp -o kccom

EXPOSE 5000

CMD ["python", "app_server.py"]