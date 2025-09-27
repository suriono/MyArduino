import requests

# Define the URL to which the GET request will be sent
url = "https://api.tomtom.com/routing/1/calculateRoute/44.742127,-93.194999:44.745049,-93.194981/json?key=5oS0hZX2lCeGfmjVWxaAbvRvGKSSlSkB&travelMode=car&sectionType=traffic&sectionType=speedLimit&report=effectiveSettings&vehicleEngineType=combustion"

# Send the GET request
response = requests.get(url)

# Accessing the response content
# The content of the response (e.g., HTML, JSON) can be accessed using .text or .json()
print(f"Status Code: {response.status_code}")
print(f"Response Content (first 200 characters): {response.text[:200]}...")

# If the response is JSON, you can parse it directly
try:
    #tt = 48
    #t2 = round 48*0.6214
    data = response.json()
    print(f"JSON Data: {data}")
    kmh = data["routes"][0]["sections"][0]["maxSpeedLimitInKmh"]
    mph = int(round(kmh * 0.6214));
    print("Speed Limit: ", mph)
except requests.exceptions.JSONDecodeError:
    print("Response is not valid JSON.")