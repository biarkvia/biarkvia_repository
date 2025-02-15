<?php

namespace App\Resource;

use App\Entity\Account;

class AccountResourceFactory
{
    public function fromEntity(Account $account): AccountResource
    {
        return new AccountResource(
            $account->getId(),
            $account->getName(),
            $account->getType(),
            $account->getExpiration()
        );
    }
}
