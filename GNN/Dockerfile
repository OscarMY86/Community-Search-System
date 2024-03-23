FROM python:3.9

WORKDIR /app

COPY requirements.txt .

RUN pip install torch==2.1.2 torchvision==0.16.2 torchaudio==2.1.2

RUN pip install --no-cache-dir -r requirements.txt

COPY . .

EXPOSE 8081

CMD ["python", "app.py"]