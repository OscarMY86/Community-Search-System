# Social Network: Community search project

## Frontend
We use React to build the frontend.
### 1. Set up up the environment
**Install node.js**
---
For Windows, install from https://nodejs.org/en/download

For Linux, 
```
curl -fsSL https://deb.nodesource.com/setup_lts.x | sudo -E bash - &&\
sudo apt-get install -y nodejs
```
Check if it can install correctly
```
node --version
npm --version
```
### 2. Start the frontend
```
cd ./comm-app
npm start
```
It will run in http://localhost:3000
### 3. Modify the frontend
Mostly change the file in src folder. Change the App.js file for the home page.

## Backend
We use Flask to build the backend server.
### Set up the virtual environment(Optional)
For more detail, see https://docs.python.org/3/library/venv.html
### 1. Set up up the environment
**Install packages**
```
pip install -r requirements.txt
```
### 2. Start the backend
Windows:
```
cd ./Backend
py app_server.py
```
Linux:
```
cd ./Backend
python3 app_server.py
```
It will run in http://localhost:5000

Folder structure
- `app_server.py` - hosting server for running the search algorithm
- `search.txt` - detail of the frontend request
- `requirements.txt` - packages for backend
- other files are for testing

> Notes: make sure you run both frontend and backend at the same time
