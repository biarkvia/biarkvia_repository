<?php

namespace App\Operation;

use App\Entity\Employee;
use Doctrine\ORM\EntityManagerInterface;
use App\Entity\Role;

class EmployeeOperation extends AbstractOperation
{
    public function __construct(EntityManagerInterface $manager)
    {
        parent::__construct($manager);
    }

    /**
     * Save or update an Employee in the database.
     *
     * @param Employee $employee
     * @return int The ID of the saved Employee.
     */
    public function store(Employee $employee): int
    {
        $this->manager->persist($employee);
        $this->manager->flush();

        return $employee->getId();
    }

    /**
     * Delete an Employee from the database.
     *
     * @param Employee $employee
     */
    public function delete(Employee $employee): void
    {
        $this->manager->remove($employee);
        $this->manager->flush();
    }

    /**
     * Find an Employee by ID.
     *
     * @param int $id
     * @return Employee|null
     */
    public function findById(int $id): ?Employee
    {
        return $this->getRepository(Employee::class)->find($id);
    }

    /**
     * Get paginated list of Employees.
     *
     * @param int $limit
     * @param int $offset
     * @return Employee[]
     */
    public function findPaginated(int $limit, int $offset): array
    {
        return $this->getRepository(Employee::class)
            ->createQueryBuilder('e')
            ->setFirstResult($offset)
            ->setMaxResults($limit)
            ->getQuery()
            ->getResult();
    }

    /**
     * Count the total number of Employees.
     *
     * @return int
     */
    public function countAll(): int
    {
        return (int) $this->getRepository(Employee::class)
            ->createQueryBuilder('e')
            ->select('COUNT(e.id)')
            ->getQuery()
            ->getSingleScalarResult();
    }

    /**
     * Find employees.
     * @param array $criteria
     * @param string $fieldOfSort
     * @param string $directionOfSort
     * @return array
     */
    public function search(array $criteria, string $fieldOfSort = 'id', string $directionOfSort = 'asc'): array
    {
        $queryBuilder = $this->getRepository(Employee::class)->createQueryBuilder('e')
            ->leftJoin('e.roles', 'r')
            ->addSelect('r');

        foreach ($criteria as $field => $value) {
            if ($field === 'roleTitles' && !empty($value)) {
                $queryBuilder->andWhere('r.title IN (:roleTitles)')
                    ->setParameter('roleTitles', $value);
            } elseif (!empty($value)) {
                $queryBuilder->andWhere(sprintf('e.%s LIKE :%s', $field, $field))
                    ->setParameter($field, '%' . $value . '%');
            }
        }

        $allowedFields = ['id', 'firstName', 'lastName', 'email', 'phone', 'website', 'description', 'role'];
        if (in_array($fieldOfSort, $allowedFields, true)) {
            if ($fieldOfSort === 'role') {
                $queryBuilder->orderBy('r.title', $directionOfSort === 'desc' ? 'DESC' : 'ASC');
            } else {
                $queryBuilder->orderBy('e.' . $fieldOfSort, $directionOfSort === 'desc' ? 'DESC' : 'ASC');
            }
        }

        return $queryBuilder->getQuery()->getResult();
    }


    /**
     * Fetch all roles from the database.
     *
     * @return Role[]
     */
    public function findAllRoles(): array
    {
        return $this->getRepository(Role::class)->findAll();
    }

    /**
     * Fetch paginated, sorted, and filtered employees.
     *
     * @param array $criteria
     * @param string $fieldOfSort
     * @param string $directionOfSort
     * @param int $page
     * @param int $limit
     * @return array Contains employees, total pages, and all roles.
     */
    public function getPaginatedEmployees(array $criteria, string $fieldOfSort, string $directionOfSort, int $page, int $limit): array
    {
        $criteria = array_filter($criteria, fn($value) => !empty($value) && $value !== []);
        $offset = ($page - 1) * $limit;

        $allFilteredEmployees = $this->search($criteria, $fieldOfSort, $directionOfSort);
        $totalEmployees = count($allFilteredEmployees);
        $employees = array_slice($allFilteredEmployees, $offset, $limit);

        $totalPages = (int) ceil($totalEmployees / $limit);
        $allRoles = $this->findAllRoles();

        return [
            'employees' => $employees,
            'totalPages' => $totalPages,
            'allRoles' => $allRoles,
        ];
    }

    /**
     * Search employees by their name.
     *
     * @param string $query
     * @return Employee[]
     */
    public function searchByName(string $query): array
    {
        if (empty($query)) {
            return [];
        }

        return $this->getRepository(Employee::class)
            ->createQueryBuilder('e')
            ->where('e.firstName LIKE :query OR e.lastName LIKE :query')
            ->setParameter('query', '%' . $query . '%')
            ->getQuery()
            ->getResult();
    }
}
