<?php

namespace App\Resource;

use DateTimeInterface;

class AccountResource
{
    public function __construct(
        public int $id,
        public string $name,
        public string $type,
        public ?DateTimeInterface $expiration
    ) {}
}
