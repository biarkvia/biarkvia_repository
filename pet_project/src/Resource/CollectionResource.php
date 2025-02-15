<?php

namespace App\Resource;

class CollectionResource
{
    public function __construct(
        public string $self,
        public int $total,
        public array $data
    ) {}
}
