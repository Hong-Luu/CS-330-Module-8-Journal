package com.contact;

public class Contact {
	private String contactID;  // unique contact ID
    private String firstName;  // first name
    private String lastName;   // last name
    private String phone;      // phone number (exactly 10 digits)
    private String address;    // address (up to 30 characters)
    
    // Constructor to initialize a contact object
    public Contact(String contactID, String firstName, String lastName, String phone, String address) {
        // Check and validate the input parameters for each field
        if (contactID == null || contactID.length() > 10) {
            throw new IllegalArgumentException("Contact ID must be non-null and up to 10 characters.");
        }
        if (firstName == null || firstName.length() > 10) {
            throw new IllegalArgumentException("First name must be non-null and up to 10 characters.");
        }
        if (lastName == null || lastName.length() > 10) {
            throw new IllegalArgumentException("Last name must be non-null and up to 10 characters.");
        }
        if (phone == null || phone.length() != 10) {
            throw new IllegalArgumentException("Phone must be exactly 10 digits and non-null.");
        }
        if (address == null || address.length() > 30) {
            throw new IllegalArgumentException("Address must be non-null and up to 30 characters.");
        }

        // initialize the contact object with the provided values
        this.contactID = contactID;
        this.firstName = firstName;
        this.lastName = lastName;
        this.phone = phone;
        this.address = address;
    }

    // Getter method for contactID
    public String getContactID() {
        return contactID;
    }

    // Getter method for firstName
    public String getFirstName() {
        return firstName;
    }

    // Setter method for firstName
    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    // Getter method for lastName
    public String getLastName() {
        return lastName;
    }

    // Setter method for lastName
    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    // Getter method for phone
    public String getPhone() {
        return phone;
    }

    // Setter method for phone
    public void setPhone(String phone) {
        this.phone = phone;
    }

    // Getter method for address
    public String getAddress() {
        return address;
    }

    // Setter method for address
    public void setAddress(String address) {
        this.address = address;
    
    }

}
