# Social Network: Community search project
## Download the dataset
Since the dataset is huge, I did not put it here. Please download the dblp.xml from https://dblp.org/faq/How+can+I+download+the+whole+dblp+dataset.html and put it in `/DBLP-Geneartion/dataset/dblp.xml`
## Frontend
We use React to build the frontend.
### 1. Set up the environment
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
Notes: If there is problem about `react-scripts start` or `comm-app@0.1.0 start`, run:
```
install npm
```
Then, start it again. It will run in http://localhost:3000
### 3. Modify the frontend
Mostly change the file in src folder. Change the App.js file for the home page.

## Backend
We use Flask to build the backend server.
### Set up the virtual environment(Optional)
For more detail, see https://docs.python.org/3/library/venv.html
> Don't push your virtual environment to GitHub
### 1. Set up the environment
**Install packages**
```
cd ./Backend
pip install -r requirements.txt
```
### 2. Start the backend
Windows:
```
py app_server.py
```
Linux:
```
python3 app_server.py
```
It will run in http://localhost:5000

Folder structure
- `app_server.py` - hosting server for running the search algorithm
- `search.txt` - detail of the frontend request
- `requirements.txt` - packages for backend
- other files are for testing

> Notes: make sure you run both frontend and backend at the same time
