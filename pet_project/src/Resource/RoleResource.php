<?php

namespace App\Resource;

class RoleResource
{
    public function __construct(
        public int $id,
        public string $title
    ) {}
}
