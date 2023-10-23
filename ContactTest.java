package com.contact;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.Before;
import org.junit.After;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

public class ContactTest {

	private Contact contact;

    @BeforeEach
    public void setUp() {
        // initialize a contact object before each test
        contact = new Contact("12345", "John", "Doe", "1234567890", "123 Main St");
    }

    @Test
    public void testContactCreation() {
        assertNotNull(contact);
    }

    @Test
    public void testInvalidContactID() {
        // attempt to create a contact with a null contactID should throw an exception
        assertThrows(IllegalArgumentException.class, () -> new Contact(null, "John", "Doe", "1234567890", "123 Main St"));
    }

    @Test
    public void testInvalidFirstName() {
        // attempt to create a contact with a null first name should throw an exception
        assertThrows(IllegalArgumentException.class, () -> new Contact("12345", null, "Doe", "1234567890", "123 Main St"));
    }

    @Test
    public void testInvalidLastName() {
        // attempt to create a contact with a null last name should throw an exception
        assertThrows(IllegalArgumentException.class, () -> new Contact("12345", "John", null, "1234567890", "123 Main St"));
    }

    @Test
    public void testInvalidPhoneNumber() {
        // attempt to create a contact with an invalid phone number (not exactly 10 digits) should throw an exception
        assertThrows(IllegalArgumentException.class, () -> new Contact("12345", "John", "Doe", "123456789", "123 Main St"));
    }

    @Test
    public void testInvalidAddress() {
        // attempt to create a contact with a null address should throw an exception
        assertThrows(IllegalArgumentException.class, () -> new Contact("12345", "John", "Doe", "1234567890", null));
    }

    @Test
    public void testGetters() {
        assertEquals("12345", contact.getContactID());
        assertEquals("John", contact.getFirstName());
        assertEquals("Doe", contact.getLastName());
        assertEquals("1234567890", contact.getPhone());
        assertEquals("123 Main St", contact.getAddress());
    }

    @Test
    public void testSetters() {
        contact.setFirstName("Jane");
        contact.setLastName("Smith");
        contact.setPhone("9876543210");
        contact.setAddress("456 Elm St");

        assertEquals("Jane", contact.getFirstName());
        assertEquals("Smith", contact.getLastName());
        assertEquals("9876543210", contact.getPhone());
        assertEquals("456 Elm St", contact.getAddress());
    }

}

