from pymongo import MongoClient
from bson.objectid import ObjectId


class Cars(object):
    """ CRUD operations for Cars collection in MongoDB """
    def __init__(self):
        # Initializing the MongoClient. This helps to
        # access the MongoDB databases and collections.
        self.client = MongoClient(host=['localhost:27017'], document_class=dict, tz_aware=False, connect=True)
        self.database = self.client['Cars']
        # Complete this create method to implement the C in CRUD.

    def create(self, data):
        """CREATE STUFF"""
        if data is not None:
            self.database.Cars.insert_one(data)  # data should be dictionary
            return True
        else:
            raise Exception("Nothing to save, because data parameter is empty")
   
    def read(self, data):
        """READ STUFF"""    
        if data is not None: 
            return self.database.Cars.find(data,{"_id":False}) # data should be dictionary
        
        else: 
            raise Exception("Nothing was found because data parameter is empty")

    def update(self, data, update):
        """UPDATE STUFF"""
        if data is not None:
            return self.database.Cars.replace_one(data, update)
        else:
            raise Exception("Nothing was found because data parameter is empty")

    def delete(self, data):
        """DELETE STUFF"""
        if data is not None:
            return self.database.Cars.delete_many(data)
        else:
            raise Exception("Nothing was found becasue data paramete is empty")

