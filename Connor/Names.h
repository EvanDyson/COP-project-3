#pragma once
#include <string>
#include <vector>
using namespace std;

//Currently around 600000 possible combinations

const vector<string> prefixes = {

        "New", "West", "East", "North", "New",
        "Saint", "Fort", "Mount", "Port", "South",
        "Grand", "Lake", "Cape"

};

const vector<string> middleWords {

    "Jackson", "Jack", "Wood", "Spring", "Red",
    "Blue", "Orange", "Yellow", "Green", "Fair",
    "Oak", "Birch", "Frank", "George", "Clint",
    "Kings", "River", "Jersey", "Adams", "Alex",
    "York", "Middle", "Columbus", "Charles", "White",
    "Troy", "Boulder", "Rock", "Garden", "Cedar",
    "Maple", "Pine", "Holly", "Elm", "Cypress",
    "Acacia", "Fir", "Hickory", "Cherry", "Ash",
    "Gray", "Chestnut", "Apple", "Magnolia", "Center",
    "Mahogany", "Carp", "Cod", "Palm", "Elder",
    "Olive", "Ivory", "Juniper", "Lance", "Lily",
    "Myrtle", "John", "James", "Thomas", "Jefferson",
    "Quincy", "Monroe", "Taylor", "Polk", "Lincoln",
    "Grant", "Chester", "Arthur", "Wilson", "Hoover",
    "Harry", "Carter", "Reagan", "Bush", "Sand",
    "Gaines", "Winter", "Silver", "Jordan", "Jones",
    "Knox", "Lewis", "Little", "Louis", "Juan",
    "Fox", "Sandy", "Scots", "Shell", "Snake",
    "Smith", "Williams", "Mary", "Stone", "Sulphur",
    "Turkey", "Brown", "Brook", "Griffin", "Wolf",
    "Day", "Night", "Sunny", "Cloudy", "Sky",
    "Grass", "Shrub", "Hedge", "Ground", "Ryan",
    "Connor", "Justin", "Caleb", "Home", "Main",
    "Cow", "Jason", "Black", "Gold", "Rose",
    "Auburn", "Vermillion", "Cerulean", "Lavender", "Violet",
    "Copper", "Wheat", "Autumn", "Water", "Arrow",
    "Amber", "Baron", "Barrow", "Briar", "Bright",
    "Chamber", "Cinder", "Clay", "Dark", "Eden",
    "Guild", "Hammer", "Kent", "Mason", "Raven",
    "Sharp", "Storm", "Marsh", "Buchanan", "Rutherford",
    "Truman", "Coral"

};

const vector<string> suffixes = {

        "City", "City", "City", "City", "City",
        "Town", "Town", "Falls", "Beach", "Summit",
        "Park", "Lake", "Bay", "River", "Haven",
        "Rock", "Island", "Creek", "Hills", "Heights",
        "Forest", "Plains", "Grove", "Village", "Ridge",
        "Bend", "Flats", "Canyon", "Springs", "Valley",
        "Point", "Fork", "Hollow", "Coast", "Pass",
        "Range", "Harbor", "Prairie", "Corner", "Edge",
        "Meadow", "Grotto", "Keep", "Rapids"

};

const vector<string> fixedSuffixes = {

    "ton", "town", "burg", "burgh", "ville",
    "wood", "ford", "berry", "beck", "bourne",
    "borough", "bury", "by", "ester", "ing",
    "combe", "cot", "dale", "den", "field",
    "forth", "ham", "hurst", "stead", "side",
    "ley", "mere", "minster", "more", "mouth",
    "mark", "port", "shaw", "thorpe", "land",
    "wich", "wick", "worth", "view", "mont",
    "dorf", "grad", "opolis", "lock", "ster",
    "boro", "burn", "bery", "ly", "furt",
    "woods", "ington", "deen", "stow", "bell",
    "sylvania", "thorn", "court", "crest", "felt",
    "helm", "loft", "mane", "shire", "shore",
    "vale",


};

/*const vector<string> standalone = {

        "Aberdeen","Abilene","Albany","Alexandria",
        "Amarillo","Arling","Arvada",
        "Augusta","Aurora","Austin","Barnstable","Billings",
        "Blooming","Braden","Bremer","Bridgeport",
        "Bryan","Burbank","Cambridge","Birmingham",
        "Canton","Cape Coral","Carroll","Cary",
        "Cedar Rapids","Chandler","Clarks","Clearwater","Concord",
        "Coral Springs","Corona","Costa Mesa","Daly City","Danbury",
        "Elizabeth","Elk Grove","Elkhart","Erie","Eugene","Fargo","Fitchburg",
        "Flint","Fontana","Fort Collins","Fort Smith","Frederick",
        "Fremont","Fresno","Fullerton","Garden Grove","Garland",
        "Gastonia","Gilbert","Glendale","Grand Prairie","Grand Rapids",
        "Grayslake","Green Bay","Greensboro","Hagerstown","Hampton",
        "Harlingen","Harrisburg","Hartford","Hayward","Henderson",
        "Hesperia","Hickory","High Point","Hollywood","Houston",
        "Howell","Huntington","Huntington Beach","Huntsville",
        "Independence","Inglewood","Irvine","Irving",
        "Lansing","Laredo","Layton","Leominster","Lewisville",
        "Lexington","Lincoln","Lorain",
        "Lowell","Lubbock","Macon","Madison","Manchester","Marina",
        "Marysville","McAllen","McHenry","Medford","Melbourne","Mesa",
        "Monroe","Monterey","Montgomery","Moreno Valley","Murrieta",
        "Newburgh","Norfolk","Norman","Norwalk","Norwich","Oakland",
        "Ontario","Orange","Overland Park","Oxnard","Palm Bay",
        "Palmdale","Pasadena","Paterson","Pensacola","Peoria",
        "Port Orange","Portsmouth","Providence","Provo","Pueblo",
        "Raleigh","Reading","Redding","Richland","Richmond","Riverside",
        "Roanoke","Rochester","Rockford","Roseville","Round Lake",
        "Seaside","Sebastian", "South Lyon","Spartanburg","Stamford",
        "Sterling Heights","Stockton","Sunnyvale","Syracuse",
        "Trenton","Tucson","Tulsa","Tuscaloosa","Tyler","Utica",
        "Vallejo","Vancouver","Vero Beach","Victorville",
        "Waterloo","West Covina","West Valley City","Westminster",
        "Wichita","Wilmington","Winston","Worcester","Yakima",
        "York","Youngstown","Buchanan","Rutherford","Truman","Coral"


};*/
