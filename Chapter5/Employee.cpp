Class Employee {

    string name;
    int age;
    float salary;
    
public : 

    string getName();
    void setName(string name);
    int getAge();    
    void setAge(int age);
    float getSalary();    
    void setSalary(float salary);   
};
    
Employee employee;
    
// Is 'employee' an object or a data structure? Why?

// The Employee class only contains getters and setters. They simply expose internal data without exposing any logic or behavior.
// Thus it is a data structure. 
// employee is thus an object of that data structure that will be used to store the data of an employee.