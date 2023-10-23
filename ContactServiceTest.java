package com.contact;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class ContactServiceTest {
    private ContactService contactService;
    private Contact contact;

    @BeforeEach
    public void setUp() {
        // initialize a ContactService and a Contact object before each test
        contactService = new ContactService();
        contact = new Contact("12345", "John", "Doe", "1234567890", "123 Main St");
    }

    @Test
    public void testAddContact() {
        // add a contact to the ContactService
        contactService.addContact(contact);

        // check if the contact was added successfully
        Contact retrievedContact = contactService.getContactByID(contact.getContactID());
        assertNotNull(retrievedContact);
        assertEquals(contact, retrievedContact);
    }

    @Test
    public void testAddDuplicateContact() {
        // add a contact with the same ID twice, which should throw an exception
        contactService.addContact(contact);

        assertThrows(IllegalArgumentException.class, () -> contactService.addContact(contact));
    }

    @Test
    public void testDeleteContact() {
        // add a contact and then delete it
        contactService.addContact(contact);
        contactService.deleteContact(contact.getContactID());

        // check if the contact was deleted successfully
        Contact retrievedContact = contactService.getContactByID(contact.getContactID());
        assertNull(retrievedContact);
    }

    @Test
    public void testDeleteNonExistentContact() {
        // attempt to delete a contact that doesn't exist, which should throw an exception
        assertThrows(IllegalArgumentException.class, () -> contactService.deleteContact("nonExistentID"));
    }

    @Test
    public void testUpdateContact() {
        // add a contact and then update its first name
        contactService.addContact(contact);
        contactService.updateContact(contact.getContactID(), "firstname", "Jane");

        // check if the contact's first name was updated successfully
        Contact retrievedContact = contactService.getContactByID(contact.getContactID());
        assertEquals("Jane", retrievedContact.getFirstName());
    }

    @Test
    public void testUpdateNonExistentContact() {
        // attempt to update a contact that doesn't exist, which should throw an exception
        assertThrows(IllegalArgumentException.class, () -> contactService.updateContact("nonExistentID", "firstname", "Jane"));
    }

    @Test
    public void testUpdateInvalidField() {
        // add a contact and then attempt to update an invalid field, which should throw an exception
        contactService.addContact(contact);

        assertThrows(IllegalArgumentException.class, () -> contactService.updateContact(contact.getContactID(), "invalidField", "NewValue"));
    }
}
