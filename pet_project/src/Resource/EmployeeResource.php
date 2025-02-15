<?php

namespace App\Resource;

class EmployeeResource
{
    public function __construct(
        public int $id,
        public string $firstName,
        public string $lastName,
        public string $email,
        public ?string $phone,
        public ?string $website,
        public ?string $description
    ) {}
}
