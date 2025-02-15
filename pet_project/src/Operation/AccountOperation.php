<?php

namespace App\Operation;

use App\Entity\Account;
use App\Entity\Employee;
use Doctrine\ORM\EntityManagerInterface;

class AccountOperation extends AbstractOperation
{
    public function __construct(EntityManagerInterface $manager)
    {
        parent::__construct($manager);
    }

    /**
     * Create or update an Account.
     *
     * @param Account $account
     * @return int The ID of the stored Account.
     */
    public function store(Account $account): int
    {
        $this->manager->persist($account);
        $this->manager->flush();

        return $account->getId();
    }

    /**
     * Delete an Account.
     *
     * @param Account $account
     */
    public function delete(Account $account): void
    {
        $this->manager->remove($account);
        $this->manager->flush();
    }

    /**
     * Find an Account by ID.
     *
     * @param int $id
     * @return Account|null
     */
    public function findById(int $id): ?Account
    {
        return $this->getRepository(Account::class)->find($id);
    }

    /**
     * Find all Accounts for a given Employee.
     *
     * @param Employee $employee
     * @return Account[]
     */
    public function findByEmployee(Employee $employee): array
    {
        return $this->getRepository(Account::class)
            ->findBy(['employee' => $employee]);
    }

    /**
     * Find all Accounts for a given Employee by criteria.
     *
     * @param array $criteria
     * @param Employee $employee
     * @return Account[]
     */
    public function findByCriteria(array $criteria, Employee $employee): array
    {
        $queryBuilder = $this->getRepository(Account::class)->createQueryBuilder('a')
            ->andWhere('a.employee = :employee')
            ->setParameter('employee', $employee);

        if (!empty($criteria['name'])) {
            $queryBuilder->andWhere('a.name LIKE :name')
                ->setParameter('name', '%' . $criteria['name'] . '%');
        }

        if (!empty($criteria['type'])) {
            $queryBuilder->andWhere('a.type LIKE :type')
                ->setParameter('type', '%' . $criteria['type'] . '%');
        }

        if (!empty($criteria['expiration'])) {
            $queryBuilder->andWhere('a.expiration = :expiration')
                ->setParameter('expiration', new \DateTime($criteria['expiration']));
        }

        return $queryBuilder->getQuery()->getResult();
    }
}
