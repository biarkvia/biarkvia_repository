<?php

namespace App\Operation;

use App\Entity\Role;
use Doctrine\ORM\EntityManagerInterface;

class RoleOperation extends AbstractOperation
{
    public function __construct(EntityManagerInterface $manager)
    {
        parent::__construct($manager);
    }

    /**
     * Create or update a Role entity in the database.
     *
     * @param Role $role
     * @return int The ID of the stored Role.
     */
    public function store(Role $role): int
    {
        $existingRole = $this->getRepository(Role::class)->findOneBy(['title' => $role->getTitle()]);

        if ($existingRole) {
            $existingRole->setDescription($role->getDescription());
            $existingRole->setIsVisible($role->isVisible());
            $role = $existingRole;
        }

        $this->manager->persist($role);
        $this->manager->flush();

        return $role->getId();
    }

    /**
     * Delete a Role entity from the database.
     *
     * @param Role $role
     */
    public function delete(Role $role): void
    {
        $this->manager->remove($role);
        $this->manager->flush();
    }

    /**
     * Find a Role entity by its ID.
     *
     * @param int $id
     * @return Role|null
     */
    public function findById(int $id): ?Role
    {
        return $this->getRepository(Role::class)->find($id);
    }

    /**
     * Retrieve all Role entities.
     *
     * @return Role[]
     */
    public function findAll(): array
    {
        return $this->getRepository(Role::class)->findAll();
    }

    /**
     * Retrieve paginated Role entities.
     *
     * @param int $limit
     * @param int $offset
     * @return Role[]
     */
    public function findPaginated(int $limit, int $offset): array
    {
        return $this->getRepository(Role::class)->findBy([], ['id' => 'ASC'], $limit, $offset);
    }

    /**
     * Count all Role entities.
     *
     * @return int
     */
    public function countAll(): int
    {
        return $this->getRepository(Role::class)->count([]);
    }

    /**
     * Search for Role entities based on criteria.
     *
     * @param array $criteria
     * @return Role[]
     */
    public function search(array $criteria): array
    {
        $queryBuilder = $this->getRepository(Role::class)->createQueryBuilder('r');

        if (!empty($criteria['title'])) {
            $queryBuilder->andWhere('r.title LIKE :title')
                ->setParameter('title', '%' . $criteria['title'] . '%');
        }

        if (!empty($criteria['description'])) {
            $queryBuilder->andWhere('r.description LIKE :description')
                ->setParameter('description', '%' . $criteria['description'] . '%');
        }

        if (isset($criteria['isVisible'])) {
            $queryBuilder->andWhere('r.isVisible = :isVisible')
                ->setParameter('isVisible', $criteria['isVisible']);
        }

        return $queryBuilder->getQuery()->getResult();
    }
}
