<?php

namespace App\Repository;

use App\Entity\Employee;
use Doctrine\Bundle\DoctrineBundle\Repository\ServiceEntityRepository;
use Doctrine\Persistence\ManagerRegistry;

/**
 * @extends ServiceEntityRepository<Employee>
 */
class EmployeeRepository extends ServiceEntityRepository
{
    public function __construct(ManagerRegistry $registry)
    {
        parent::__construct($registry, Employee::class);
    }

    /**
     * Find a paginated list of employees.
     *
     * @param int $limit The number of results per page.
     * @param int $offset The starting point for the results.
     * @return Employee[] Returns an array of Employee objects.
     */
    public function findPaginatedEmployees(int $limit, int $offset): array
    {
        return $this->createQueryBuilder('e')
            ->setFirstResult($offset)
            ->setMaxResults($limit)
            ->getQuery()
            ->getResult();
    }

    /**
     * Count the total number of employees.
     *
     * @return int The total number of employees.
     */
    public function countAllEmployees(): int
    {
        return $this->count([]);
    }

    /**
     * Find the latest employees.
     *
     * @param int $limit The number of employees to retrieve.
     * @return Employee[] Returns an array of the latest Employee objects.
     */
    public function findLatestEmployees(int $limit): array
    {
        return $this->createQueryBuilder('e')
            ->orderBy('e.id', 'DESC')
            ->setMaxResults($limit)
            ->getQuery()
            ->getResult();
    }

    /**
     * Get only visible roles for a given employee.
     *
     * @param Employee $employee The employee entity.
     * @return array Returns an array of visible Role objects.
     */
    public function findEmployeesWithVisibleRoles(int $limit, int $offset): array
    {
        return $this->createQueryBuilder('e')
            ->join('e.roles', 'r')
            ->where('r.isVisible = :visible')
            ->setParameter('visible', true)
            ->setFirstResult($offset)
            ->setMaxResults($limit)
            ->getQuery()
            ->getResult();
    }
}
