public class CustomerSearch
{
    public List < Customer > SearchByCountry(string country)
    {    
        var queriedCustomer = SearchCustomer(country);
    }

    public List < Customer > SearchByCompanyName(string company)
    {
        var queriedCustomer = SearchCustomer(company);
    }

    public List < Customer > SearchByContact(string contact)
    {   
        var queriedCustomer = SearchCustomer(contact);
    }

    private List<Customer> SearchCustomer(string searchFactor){
        var queriedCustomer = from customer in database.customers where customer.Country.Contains(searchFactor) orderby customer.CustomerID ascending select customer;
        return queriedCustomer.ToList();
    }
}

public class CustomerDataExporter
{
    public string ExportCustomerDataToCSV(List < Customer > customerData)
    {

        StringBuilder stringBuilder = new StringBuilder();

        foreach(var customer in customerData)
        {
            stringBuilder.AppendFormat("{0}, {1}, {2}, {3}", customer.CustomerID, customer.CompanyName, customer.ContactName, customer.Country);
            stringBuilder.AppendLine();
        }
        return stringBuilder.ToString();
    }
} 
