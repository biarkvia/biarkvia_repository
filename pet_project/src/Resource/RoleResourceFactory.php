<?php

namespace App\Resource;
class RoleResourceFactory
{
    public function fromEntity(Role $role): RoleResource
    {
        return new RoleResource(
            $role->getId(),
            $role->getTitle()
        );
    }
}