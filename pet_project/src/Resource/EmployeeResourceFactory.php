<?php

namespace App\Resource;

use App\Entity\Employee;

class EmployeeResourceFactory
{
    public function fromEntity(Employee $employee): EmployeeResource
    {
        return new EmployeeResource(
            $employee->getId(),
            $employee->getFirstName(),
            $employee->getLastName(),
            $employee->getEmail(),
            $employee->getPhone(),
            $employee->getWebsite(),
            $employee->getDescription()
        );
    }
}
