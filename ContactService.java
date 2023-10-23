package com.contact;

import java.util.ArrayList;
import java.util.List;

public class ContactService {
    private List<Contact> contacts;  // List to store contacts

    // constructor to initialize the contacts list
    public ContactService() {
        this.contacts = new ArrayList<>();
    }

    // method to add a contact with a unique ID
    public void addContact(Contact contact) {
        // check if a contact with the same ID already exists
        if (getContactByID(contact.getContactID()) != null) {
            throw new IllegalArgumentException("Contact with the same ID already exists.");
        }
        
        // add the contact to the list
        contacts.add(contact);
    }

    // method to delete a contact by contact ID
    public void deleteContact(String contactID) {
        // find the contact by ID
        Contact contactToDelete = getContactByID(contactID);

        // check if the contact exists
        if (contactToDelete != null) {
            // remove the contact from the list
            contacts.remove(contactToDelete);
        } else {
            throw new IllegalArgumentException("Contact with the specified ID not found.");
        }
    }

    // method to update contact fields by contact ID
    public void updateContact(String contactID, String fieldToUpdate, String newValue) {
        // find the contact by ID
        Contact contactToUpdate = getContactByID(contactID);

        // check if the contact exists
        if (contactToUpdate != null) {
            // update the specified field
            switch (fieldToUpdate.toLowerCase()) {
                case "firstname":
                    contactToUpdate.setFirstName(newValue);
                    break;
                case "lastname":
                    contactToUpdate.setLastName(newValue);
                    break;
                case "number":
                    contactToUpdate.setPhone(newValue);
                    break;
                case "address":
                    contactToUpdate.setAddress(newValue);
                    break;
                default:
                    throw new IllegalArgumentException("Invalid field name for updating.");
            }
        } else {
            throw new IllegalArgumentException("Contact with the specified ID not found.");
        }
    }

    // method to get a contact by contact ID
    public Contact getContactByID(String contactID) {
        for (Contact contact : contacts) {
            if (contact.getContactID().equals(contactID)) {
                return contact;
            }
        }
        return null; // contact not found
    }
 
}
