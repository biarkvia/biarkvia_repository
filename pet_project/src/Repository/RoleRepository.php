<?php

namespace App\Repository;

use App\Entity\Role;
use Doctrine\Bundle\DoctrineBundle\Repository\ServiceEntityRepository;
use Doctrine\Persistence\ManagerRegistry;

/**
 * @extends ServiceEntityRepository<Role>
 */
class RoleRepository extends ServiceEntityRepository
{
    public function __construct(ManagerRegistry $registry)
    {
        parent::__construct($registry, Role::class);
    }

    /**
     * Return all roles that are visible.
     *
     * @return Role[]
     */
    public function findVisibleRoles(): array
    {
        return $this->createQueryBuilder('r')
            ->andWhere('r.isVisible = :isVisible')
            ->setParameter('isVisible', true)
            ->getQuery()
            ->getResult();
    }
}